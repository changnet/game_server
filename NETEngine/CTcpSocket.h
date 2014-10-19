#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include <ev++.h>

#include "gstypes.h"
#include "gssocket.h"
#include "CMessageQueue.h"

//这个类型是按位设置
enum ENetStatus
{
    ES_NONE       = 0x00,             //未使用状态
    ES_CONNECTED  = 0x01,             //已连接，从未收到数据
    ES_ACTIVITY   = 0x02,             //活动的连接
    ES_DISCONNECT = 0x04              //客户端已断开的连接
};

class CTcpSocket
{
public:
    CTcpSocket();
    ~CTcpSocket();

    int32 set_socket_fd( int32 fd );
    int32 get_socket_fd();

    int64 set_connect_time( int64 _time );
    int64 get_connect_time( );

    ENetStatus get_socket_status();
    ENetStatus set_socket_status( ENetStatus status );

    struct sockaddr_in set_socket_address(const sockaddr_in address );

    void wait_close();
    void close_socket();
    void init_socket();
    void uninit_socket();
    void start();

private:
    void io_read_cb(ev::io &w, int32 revents );
    void process_io_read();

    void commit_user_msg();

private:
    int32 m_socket_fd;
    CNetMessage *m_prmessage;
    CNetMessage *m_pwmessage;
    struct sockaddr_in m_address;
    ENetStatus m_status;
    CMessageQueue *m_pmessage;
    int64 m_connect_time;

    void *m_hander_ptr;

    ev::io m_read_watcher;
};

#endif // CTCPSOCKET_H
