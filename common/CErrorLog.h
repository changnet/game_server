#ifndef CERRORLOG_H
#define CERRORLOG_H

#include <iostream>
#include "gstypes.h"
#include "CProcess.h"

#define GERRORFILE    "error.log"
#define FILEFLAG      "ab+"


class CErrorLog
{
public:
    static CErrorLog *instance()
    {
        if ( null == m_plogger )
            m_plogger = new CErrorLog( GERRORFILE );

        return m_plogger;
    }

    static void uninstance();

    static CErrorLog &log( )
    {
        CErrorLog *pLog = instance();  //do not try pLog = m_plogger

        *pLog << CProcess::get_log_process_id();

        return *pLog;
    }

    friend inline CErrorLog &operator << ( CErrorLog &log,const char *buff )
    {

#ifdef DEBUG
        std::cout << buff;
#endif

        std::cerr << buff;

        return log;
    }

    friend inline CErrorLog &operator << ( CErrorLog &log,const string &buff )
    {

#ifdef DEBUG
        std::cout << buff;
#endif

        std::cerr << buff;

        return log;
    }

private:
    CErrorLog(const string _path );
    ~CErrorLog();

    string m_file_path;
    static CErrorLog *m_plogger;
};

#endif // CERRORLOG_H
