#include <sys/types.h>
#include <unistd.h>

#include "CLogFile.h"

#include "CUtility.h"

extern const char* ENGINE_NAME;

CLogFile *CLogFile::m_plog_file = null;

CLogFile::CLogFile()
{
}

CLogFile &CLogFile::instance()
{
    if ( null == m_plog_file )
        m_plog_file = new CLogFile();

    return *m_plog_file;
}

void CLogFile::unistance()
{
    if ( m_plog_file )
        delete m_plog_file;

    m_plog_file = null;
}

std::ostream &CLogFile::error()
{
    std::freopen ( GERRORFILE,FILEFLAG,stderr );  //redirect cerr

    std::cerr << ENGINE_NAME << getpid() << "@" << CUtility::instance()->str_time() << ":";

    return std::cerr;
}

std::ostream &CLogFile::fatal()
{
    std::freopen ( GFATALFILE,FILEFLAG,stderr );  //redirect cerr

    std::cerr << ENGINE_NAME << getpid() << "@" << CUtility::instance()->str_time() << ":";

    return std::cerr;
}

std::ostream &CLogFile::warning()
{
    std::freopen ( GWARNINGFILE,FILEFLAG,stderr );  //redirect cerr

    std::cerr << ENGINE_NAME << getpid() << "@" << CUtility::instance()->str_time() << ":";

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
