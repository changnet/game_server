#include "CListenSocket.h"

#include "gslog.h"

CListenSocket::CListenSocket()
{
    m_socket_fd = 0;
    m_port = 0;
}

int32 CListenSocket::get_listen_socket_fd()
{
    return m_socket_fd;
}

uint32 CListenSocket::get_listen_socket_port()
{
    return m_port;
}

uint32 CListenSocket::set_listen_socket_port( uint32 port )
{
    return m_port = port;
}

bool CListenSocket::init_listen()
{
    if ( 0 == m_port )
    {
        GERROR() << "no listenning port was specified,server will run without listenning\n";
        return false;
    }

    struct sockaddr_in sk_socket;
    int32 optval = 1;


    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( -1 == m_socket_fd )
    {
        GERROR() << "NETEngine create listenning socket fail:" << strerror(errno) << "\n";

        close( m_socket_fd );

        return false;
    }

    /*
     * enable address reuse.it will help when the socket is in TIME_WAIT status.
     * for example:
     *     server crash down and the socket is still in TIME_WAIT status.if try
     * to restart server immediately,you need to reuse address.but note you may
     * receive the old data from last time.
     */
    if ( setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR,(char *) &optval, sizeof(optval)) == -1)
    {
        GERROR() << "NETEngine set listenning socket address reuse fail:" << strerror(errno) << "\n";

        close( m_socket_fd );

        return false;
    }

    memset( &sk_socket,'\0',sizeof(sk_socket) );
    sk_socket.sin_family = AF_INET;
    sk_socket.sin_addr.s_addr = INADDR_ANY;
    sk_socket.sin_port = htons( m_port );

    if ( bind( m_socket_fd, (struct sockaddr *) & sk_socket,sizeof(sk_socket)) == -1)
    {
        GERROR() << "NETEngine bind listenning socket fail:" << strerror(errno) << "\n";

        close( m_socket_fd );

        return false;
    }

    if ( set_socket_block_status( m_socket_fd, true ) == -1 )
    {
        GERROR() << "NETEngine bind listenning socket fail:" << strerror(errno) << "\n";

        close( m_socket_fd );

        return false;
    }

    if ( listen( m_socket_fd, PENDING ) == -1)
    {
        GERROR() << "NETEngine listenning socket listen fail:" << strerror(errno) << "\n";

        close( m_socket_fd );

        return false;
    }

    return true;
}
