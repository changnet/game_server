#ifndef CLISTENSOCKET_H
#define CLISTENSOCKET_H

#include <ev++.h>
#include "gstypes.h"
#include "gssocket.h"

class CListenSocket
{
public:
    CListenSocket();

    bool init_listen();
    int32 get_listen_socket_fd();
    uint32 get_listen_socket_port();
    uint32 set_listen_socket_port( uint32 port );

private:
    int32 m_socket_fd;
    uint32 m_port;
};

#endif // CLISTENSOCKET_H
