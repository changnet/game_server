#ifndef CNETBACKEND_H
#define CNETBACKEND_H

#include <vector>
#include "CListenSocket.h"
#include "CTcpSocket.h"

using std::vector;

class CNetBackend
{
public:
    CNetBackend();
    ~CNetBackend();

    void start_work();
    void end_work();

    static void abort_work( const char *msg );

private:    //function
    void accept_cb( ev::io &w,int32 revents );
    void socket_check_cb( ev::timer &w,int32 revents );
    void add_socket(int32 fd, const sockaddr_in address );

private:
    CListenSocket *m_plistenning_socket;
    struct ev_loop *loop;

    ev::io w_listen;
    ev::timer w_socket_check;

    vector<CTcpSocket *> m_tcp_sockets;  //socket_fd作为下标
    int32 m_online_sockets;              //当前在线人数，即m_tcp_sockets中的有效元素

};

#endif // CNETBACKEND_H
