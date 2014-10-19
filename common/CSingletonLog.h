#ifndef CSINGLETONLOG_H
#define CSINGLETONLOG_H

#include "CLogger.h"
#include "CProcess.h"

class CSingletonLog : public CLogger
{
public:
    static CSingletonLog *instance()
    {
        if ( null == m_plogger )
            m_plogger = new CSingletonLog();

        return m_plogger;
    }

    static void uninstance();

    static CSingletonLog &log( const string _path )
    {
        CSingletonLog *pLog = instance();  //do not try pLog = m_plogger

        pLog->set_log_file_path( _path );

        return *pLog;
    }

    static CSingletonLog &id_log( const string _path )
    {
         return log( _path ) << CProcess::get_log_process_id();
    }

    friend inline CSingletonLog &operator << ( CSingletonLog &log,const char *buff )
    {
        log.write_log( buff );
        log.close_file();  //if close_log,will clear file path,INFO(x)<<str<<str will fail

        return log;
    }

    friend inline CSingletonLog &operator << ( CSingletonLog &log,const string &buff )
    {
        log.write_log( buff );
        log.close_file();    //if close_log,will clear file path,INFO(x)<<str<<str will fail

        return log;
    }

private:
    CSingletonLog();
    ~CSingletonLog();

    static CSingletonLog *m_plogger;
};

#endif // CSINGLETONLOG_H
