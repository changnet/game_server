#include "CLogStream.h"
#include "gstypes.h"
#include "fatal.h"

/**
 * @brief CLogStream::CLogStream
 * @param file
 * 构造函数，打开文件
 */
CLogStream::CLogStream(const char *file)
{
    snprintf( m_path,MAX_LOG_PATH_LEN,"%s",file );
}

/**
 * @brief CLogStream::open
 * 打开日志文件
 */
void CLogStream::open()
{
    m_file.open( m_path, std::ofstream::out | std::ofstream::app );
    if ( m_file.fail() ) //print why
    {
        fatal( "open runtime stream file fail",errno );/* already exit　*/
        return;
    }
}

/**
 * @brief CLogStream::close
 * 关闭文件
 */
void CLogStream::close()
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

    open();
    m_file << "\n";
    close();

    return *this;
}
