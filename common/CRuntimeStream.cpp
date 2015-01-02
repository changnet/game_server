#include "CRuntimeStream.h"
#include "gstypes.h"
#include <cstdio>
#include <cstdlib>

/**
 * @brief CRuntimeStream::CRuntimeStream
 * 打开文件句柄
 */
CRuntimeStream::CRuntimeStream( const char *file )
{
    m_file.open( file, std::ofstream::out | std::ofstream::app );
    if ( m_file.fail() ) //print why
    {
        perror( "open runtime log file fail:" );
        exit ( EXIT_FAILURE );
    }
}

/**
 * @brief CRuntimeStream::~CRuntimeStream
 * 关闭文件
 */
CRuntimeStream::~CRuntimeStream()
{
    m_file.close();
}

/**
 * @brief CRuntimeStream::operator <<
 * @return
 * 重载operator << 以使用std::endl
 */
CRuntimeStream &CRuntimeStream::operator << ( std::ostream& (*pf)(std::ostream&) )
{
    pf( std::cerr );
    m_file << "\n";

    return *this;
}
