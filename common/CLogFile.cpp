#include "CLogFile.h"
#include <cstdio>    //for stderr

CLogFile::CLogFile()
{
}

std::ostream &CLogFile::error()
{
    std::freopen ( GERRORFILE,FILEFLAG,stderr );  //redirect cerr

    return std::cerr;
}

std::ostream &CLogFile::fatal()
{
    std::freopen ( GFATALFILE,FILEFLAG,stderr );  //redirect cerr

    return std::cerr;
}

CLogFile &CLogFile::log_file(const string &path)
{
    if ( m_file.is_open() )  //last time not close
        m_file.close();

    m_file.open( path.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary );

    if ( m_file.fail() ) //print why
    {
        error() << "open file \"" << path << "\" fail:" << strerror( errno ) << "\n";
    }

    //如果打开失败，则<<操作什么都不做
    return *this;
}

template<typename T>
CLogFile &CLogFile::operator << ( T &t )
{
    m_file << t;

    if ( m_file.fail() || m_file.bad() )  //see http://www.cplusplus.com/reference/ostream/ostream/operator<</
    {
        error() << "write file<" << m_path << "> fail:" << strerror( errno ) << "\n";
    }

    return *this;
}

/**
 * @brief CLogFile::operator <<
 * endl对于log而言表示本次log结束，关闭文件
 * 换行请用\n
 * see http://www.cplusplus.com/reference/ostream/endl/?kw=endl
 */
void CLogFile::operator << ( std::ostream& (*pf)(std::ostream&) )
{
    pf( m_file );
    m_file.close();
}
