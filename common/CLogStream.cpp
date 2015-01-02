#include "CLogStream.h"
#include "gstypes.h"

#include <cstdio>
#include <cstdlib>

/**
 * @brief CLogStream::CLogStream
 * @param file
 * 构造函数，打开文件
 */
CLogStream::CLogStream(const char *file)
{
    m_file.open( file,std::ofstream::out | std::ofstream::app );

    if ( m_file.fail() ) //print why
    {
        perror( "open runtime log file fail:" );
        exit ( EXIT_FAILURE );
    }
}

/**
 * @brief CLogStream::~CLogStream
 * 关闭文件
 */
CLogStream::~CLogStream()
{
    m_file.close();
}

/**
 * @brief CLogStream::operator <<
 * @return
 * 重载以使用std::endl
 */
CLogStream &CLogStream::operator << ( std::ostream& (*pf)(std::ostream&) )
{
#ifdef DEBUG
    pf( std::cerr );
#else
    UNUSED( pf );    /* avoid warning Wunused */
#endif

    m_file << "\n";

    return *this;
}
