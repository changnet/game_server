#ifndef FATAL_H
#define FATAL_H

#include <cstdio>        /* for freopen */
#include <cstdlib>       /* for exit */

/**
 * @brief fatal
 * @param msg
 * @param _errno
 * 严重错误日志，当error日志流都无法使用时启用
 */
static inline void fatal( const char *msg,int _errno = 0 )
{
    freopen( "fatal","wb+",stderr );

    std::cerr << msg;
    if ( _errno )
        std::cerr << ":" << strerror( _errno );
    std::cerr << std::endl;

    fclose( stderr );

    abort();
}

#endif // FATAL_H
