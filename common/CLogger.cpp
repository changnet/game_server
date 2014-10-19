#include "CLogger.h"
#include "gslog.h"
#include <exception>
#include <cstdio>
#include <cerrno>
#include <cstring>

CLogger::CLogger()
{
}

CLogger::CLogger(const string _path)
{
    m_file_path = _path;
}

CLogger::~CLogger()
{
    close_log();
}

bool CLogger::close_log()
{
    m_file_path.clear();

    if ( m_file.is_open() )
    {
        m_file.close();  //close file will fflush
        return true;
    }

    return false;
}

/*
 * close file stream but not clear file path so can open again
 */
bool CLogger::close_file()
{
    if ( m_file.is_open() )
    {
        m_file.close();  //close file will fflush
        return true;
    }

    return false;
}


bool CLogger::open_log()
{
    if ( m_file_path.empty() )  //whether its length is 0
        return false;


    m_file.open ( m_file_path.c_str(), ofstream::out | ofstream::app | ofstream::binary );

    if ( m_file.fail() ) //print why
    {
        GERROR() << "open file \"" << m_file_path << "\" fail:" << strerror( errno ) << "\n";
        return false;
    }

    return true;
}

void CLogger::flush()
{
    if ( m_file.is_open() )
        m_file.flush( );
}

const string CLogger::get_log_file_path()
{
    return m_file_path;
}

const string CLogger::set_log_file_path( const string _path )
{
    return m_file_path = _path;
}

bool CLogger::write_log(const char *buff)
{
    if ( m_file.is_open() || open_log() )
    {
        int32 len = strlen( buff );
        if ( len > MAXLOGLENGTH )
        {
            char szTruncation[ MAXLOGLENGTH ];
            memcpy( szTruncation,buff,MAXLOGLENGTH - 1 );
            szTruncation[ MAXLOGLENGTH ] = '\0';

            GERROR() << "log buff overflow,string truncation:" << szTruncation << "\n";
            len = MAXLOGLENGTH;
        }

        m_file.write( buff,len );

        if ( !m_file.good() )    //write file error
        {
            GERROR() << "write file error:" << strerror( errno ) <<"\n";
            return false;
        }

        return true;
    }

    return false;
}

bool CLogger::write_log( const string buff )
{
    if ( m_file.is_open() || open_log() )
    {
        if ( buff.size() > MAXLOGLENGTH )
        {
            const string substring = buff.substr( 0,MAXLOGLENGTH );
            GERROR() << "log buff overflow,string truncation:" << substring << "\n";
            m_file << substring;
        }
        else
            m_file << buff;

        if ( !m_file.good() )    //write file error
        {
            GERROR() << "write file error:" << strerror( errno ) <<"\n";
            return false;
        }

        return true;
    }

    return false;
}



