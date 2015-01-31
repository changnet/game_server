#include "CRuntimeStream.h"
#include "gstypes.h"
#include "fatal.h"

/**
 * @brief CRuntimeStream::CRuntimeStream
 * 打开文件句柄
 */
CRuntimeStream::CRuntimeStream( const char *file )
{
    snprintf( m_path,MAX_RUNTIME_PATH_LEN,"%s",file );
}

/**
 * @brief CRuntimeStream::open
 * 打开日志文件
 */
void CRuntimeStream::open()
{
    m_file.open( m_path, std::ofstream::out | std::ofstream::app );
    if ( m_file.fail() ) //print why
    {
        fatal( "open runtime stream file fail",errno );/* already exit　*/
        return;
    }
}

/**
 * @brief CRuntimeStream::close
 * 关闭文件
 */
void CRuntimeStream::close()
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

    open();
    m_file << "\n";
    close();

    return *this;
}
