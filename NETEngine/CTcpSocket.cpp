#include "CTcpSocket.h"

#include <cstring>
#include <cerrno>
#include "gslog.h"

CTcpSocket::CTcpSocket()
{
    m_status    = ES_NONE;
    m_socket_fd = 0;
    m_prmessage = null;
    m_pwmessage = null;
    m_pmessage  = CMessageQueue::instance();
}

CTcpSocket::~CTcpSocket()
{
    close_socket();
}

void CTcpSocket::close_socket()
{
    if ( m_socket_fd )
        close( m_socket_fd );

    m_socket_fd = 0;
}

int32 CTcpSocket::set_socket_fd(int32 fd)
{
    return m_socket_fd = fd;
}

int32 CTcpSocket::get_socket_fd()
{
    return m_socket_fd;
}

int64 CTcpSocket::get_connect_time()
{
    return m_connect_time;
}

int64 CTcpSocket::set_connect_time( int64 _time )
{
    return m_connect_time = _time;
}

void CTcpSocket::init_socket()
{
    //通信socket禁止reuse端口，以免仍处于CLOSE_WAIT的socket被重用
    set_socket_block_status( m_socket_fd,true );
    open_socket_keepalive( m_socket_fd );
    set_socket_user_timeout( m_socket_fd );

    //申请读写缓冲区
    m_prmessage = m_pmessage->get_free_msg();
    m_pwmessage = m_pmessage->get_free_msg();

    m_prmessage->zero();
    m_pwmessage->zero();
}

//将socket反初始化到new状态
void CTcpSocket::uninit_socket()
{
    m_read_watcher.stop();

    close_socket();
    m_status = ES_NONE;

    //如果申请了缓冲区，需要归还
    if ( m_prmessage )
        m_pmessage->add_free_msg( m_prmessage );

    if ( m_pwmessage )
        m_pmessage->add_free_msg( m_pwmessage );

    m_socket_fd = 0;
    m_prmessage = null;
    m_pwmessage = null;
    m_hander_ptr = null;

    memset( (char *)&m_address,'\0',sizeof( m_address ) );
}

struct sockaddr_in CTcpSocket::set_socket_address(const sockaddr_in address)
{
    memcpy( (char *)&m_address,(char *)&address,sizeof(m_address) );

    return m_address;
}

ENetStatus CTcpSocket::get_socket_status()
{
    return m_status;
}

ENetStatus CTcpSocket::set_socket_status( ENetStatus status )
{
    return m_status = status;
}

void CTcpSocket::io_read_cb(ev::io &w, int32 revents)
{
    if ( EV_ERROR & revents )
    {
        GERROR() << "io_read_cb error:" << strerror( errno ) << "\n";

        w.stop();

        return;
    }

    int32 ret = read( m_socket_fd,m_prmessage->buff,BUFF_LENGTH - m_prmessage->get_length() );
    /*一次应该能读完,所设置BUFF大于socket默认缓冲区
     * 如果未读完或其他原因，则libev在会再次触发
     */

    if ( ret < 0 )  //error
    {
        if ( EAGAIN == errno || EWOULDBLOCK == errno )  //不处理，等待下次触发再重新读取数据
        {
            GWARNING() << "read socket found EAGAIN or EWOULDBLOCK error\n";
            return;
        }

        wait_close();//close socket
    }
    else if ( 0 == ret ) //disconnect
    {
        /* 不能直接关闭socket，需要通知游戏服该socket关闭
         * 服务器未close，则socket处于CLOSE_WAIT状态，超时为keep-alive时间
         * 这段时间足够游戏服关闭socket
         */
        wait_close();
    }
    else    //read data
    {
        m_prmessage->add_length( ret ); //should nerver bigger than BUFF_LENGTH

        process_io_read();
    }
}

void CTcpSocket::start()
{
    m_read_watcher.set<CTcpSocket,&CTcpSocket::io_read_cb>(this);
    m_read_watcher.start( m_socket_fd,ev::READ );
}

/* 对数据进行粘包处理
 * 要求协议格式 长度[uint16]+数据，其中长度为数据长度，不包含自己
 * 一旦发现粘包，将把下一个包数据分离出来
 * copy到另一个buff
 *
 * 这里PACKET_HEAD_LEN用sizeof(uint16)更安全
 * 如更改协议，请注意修改PACKET_HEAD_LEN
 */
void CTcpSocket::process_io_read()
{
    uint32 current_length = m_prmessage->get_length();
    if ( current_length < PACKET_HEAD_LEN )  //not even read packet head
        return;

    uint16 length = *( reinterpret_cast<uint16 *>(m_prmessage->buff) );
    //this may faster than memcpy( &length,m_prmessage->buff,PACKET_HEAD_LEN );

    if ( length > BUFF_LENGTH )  //协议过长或非法协议
    {
        GERROR() << "socket buffer overflow or illegal packet,current socket close\n";
        return;
    }

    length += PACKET_HEAD_LEN;  //数据长度不包含协议长度，才需要加

    if ( length > current_length )  //协议不完整
        return;
    else if ( length == current_length )  //刚好，无粘包
    {
        commit_user_msg( );  //放到消息队列缓存

        m_prmessage = m_pmessage->get_free_msg();  //重新获取缓冲区
        m_prmessage->zero();  //zero更快，truncate()更安全

        return;
    }
    else                    //处理粘包
    {
        CNetMessage *pmsg = m_pmessage->get_free_msg();  //重新获取缓冲区
        pmsg->zero();

        int32 nbuff = current_length - length;  //下一个包已读取的数据长度

        memcpy( pmsg->buff,m_prmessage->buff + length,nbuff );//拷贝下一个包的数据到另一个缓冲区
        pmsg->add_length( nbuff );

        m_prmessage->set_length( length ); //缩短当前缓冲区长度刚好为协议长度
        commit_user_msg( );

        m_prmessage = pmsg;  //改当前socket缓冲区为下个包的缓冲区继续接收数据
        process_io_read( );  //重新处理当前包
    }
}

/* 当客户端close时，服务端进入CLOSE_WAIT
 * 此时不能立即close()，需要先游戏服报告该
 * socket关闭。在收到游戏服应答后才close
 *
 * 否则网络引擎先关闭，游戏服未关闭。这时恰好
 * 有新用户使用该连接，造成数据混乱
 */
void CTcpSocket::wait_close()
{
    close_socket();
}

/**
 * @brief CTcpSocket::commit_user_msg
 *
 * 向消息队列提交从网络读取完整的消息
 *
 */
void CTcpSocket::commit_user_msg()
{
    m_prmessage->set_file_description( m_socket_fd );
    m_prmessage->set_msg_param( ENMP_USER|ENMP_SOCKE );  //设定为socket fd

    m_pmessage->push_write_cache_msg( m_prmessage );  //放到消息队列缓存

    m_prmessage = null;
}
