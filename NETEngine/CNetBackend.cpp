#include "CNetBackend.h"

#include <cerrno>

#include "CServerSetting.h"
#include "gslog.h"

CNetBackend::CNetBackend()
{
    m_online_sockets = 0;
    m_tcp_sockets.assign( DEFAULT_SOCKETS,null );
    m_plistenning_socket = new CListenSocket();
    loop = EV_DEFAULT; //EV_DEFAULT每次返回的对象是同一个,相当于单例

    ev_set_syserr_cb( abort_work );
}

CNetBackend::~CNetBackend()
{
}

void CNetBackend::start_work()
{
    uint32 port = static_cast<uint32>( CServerSetting::get_listen_port() );
    m_plistenning_socket->set_listen_socket_port( port );

    if ( m_plistenning_socket->init_listen() )  //不管成功或失败，顶多程序就是不监听而已.失败在error.log有记录
    {
        w_listen.set<CNetBackend,&CNetBackend::accept_cb>(this);

        int32 fd = m_plistenning_socket->get_listen_socket_fd();
        w_listen.start( fd,ev::READ );

        /*
         * The constructor (optionally) takes an event loop to associate the watcher with.
         * If it is omitted, it will use EV_DEFAULT.
         * w->set (loop)
         *     Associates a different struct ev_loop with this watcher.
         * You can only do this when the watcher is inactive (and not pending either).
         */
        //w_listen.set( loop );
    }

    w_socket_check.set<CNetBackend,&CNetBackend::socket_check_cb>(this);
    w_socket_check.set( DISCONNECT_TIMEOUT,DISCONNECT_TIMEOUT );
    w_socket_check.start();

    ev_run( loop,0 );
}

/*
 * If your compiler can inline your callback (i.e. it is visible to it at the place of the set call and
 * your compiler is good :), then the method will be fully inlined into the thunking function, making it
 * as fast as a direct C callback.
 *
 * revents was bitset
 *
 * if (revents & EV_READ ) fd->revents |= fd->events & POLLIN;
 * if (revents & EV_WRITE) fd->revents |= fd->events & POLLOUT;
 */
void CNetBackend::accept_cb(ev::io &w, int32 revents)
{
    if ( EV_ERROR & revents )  //error
    {
        GERROR() << "accept_cb error:" << strerror(errno) << "\n";

        w.stop();

        return;
    }

    struct sockaddr_in socket_address;
    int32 length = sizeof( socket_address );

    //w.fd is m_plistenning_socket->get_listen_socket_fd()
    int32 fd = accept( w.fd,(sockaddr *)&socket_address,(socklen_t *)&length );
    if ( fd < 0 )
    {
        if ( EWOULDBLOCK == errno || EAGAIN == errno )  //无连接libev怎么会调用accept_cb
        {
            GERROR() << "accept_cb EWOULDBLOCK or EAGAIN found\n";
            return;
        }

        GERROR() << "accept new connection fail:" << strerror( errno ) << "\n";

        return;
    }

    add_socket( fd,socket_address );
}

void CNetBackend::end_work()
{
}

void CNetBackend::abort_work(const char *msg)
{
    perror (msg);
    abort ();
}

void CNetBackend::add_socket( int32 fd,const struct sockaddr_in address )
{
    if ( fd > MAX_FD )
    {
        GERROR() << "add_socket MAX_FD limit\n";

        return;

    }

    while ( (fd > static_cast<int32>(m_tcp_sockets.size()-1)) && (MAX_FD > m_tcp_sockets.size()) )  //-1数组下标从0开始
    {
        m_tcp_sockets.resize( DEFAULT_SOCKETS,null );
        GWARNING() << "socket vector resize occured\n";
    }

    CTcpSocket *psocket = m_tcp_sockets[fd];
    if ( null == psocket )
    {
        m_tcp_sockets[fd] = new CTcpSocket();//该构造函数初始化变量
        psocket = m_tcp_sockets[fd];
    }

    psocket->uninit_socket();  //clean old data

    psocket->set_socket_fd( fd );
    psocket->set_socket_address( address );

    psocket->init_socket();

    psocket->set_socket_status( ES_CONNECTED );
    psocket->set_connect_time( ev_now(loop) );  //use ev_now() instead of time()

    psocket->start();
}

void CNetBackend::socket_check_cb(ev::timer &w, int32 revents)
{
    if ( EV_ERROR & revents )
    {
        GFATAL() << "check socket connect timeout call back error:" << strerror(errno) << "\n";
        w.stop();

        abort_work( "socket_check_cb fail" );
    }

    int64 _now = ev_now(loop);
    CTcpSocket *psocket = null;

    vector<CTcpSocket *>::iterator itr = m_tcp_sockets.begin();
    while ( itr != m_tcp_sockets.end() )
    {
        psocket = *itr;

        itr ++;

        if ( !psocket )  //this fd not use
            continue;

        if ( ES_CONNECTED == psocket->get_socket_status()
             && _now - psocket->get_connect_time() > DISCONNECT_TIMEOUT )
        {
            psocket->close_socket();  //该状态未与游戏服建立连接，可以直接关闭
            psocket->set_socket_status( ES_NONE );
        }
    }
}
