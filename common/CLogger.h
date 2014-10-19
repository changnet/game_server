#ifndef CLOGGER_H
#define CLOGGER_H

#include <iostream>
#include <fstream>
#include "gstypes.h"

using std::cerr;
using std::endl;
using std::ofstream;

#define MAXLOGLENGTH    3000           //max character for one time write to log file
#define ENDLOG    false                    //log end,CLogger will flush ofstream and close log file

class CLogger
{
public:
    CLogger();
    CLogger( const string _path );

    ~CLogger();

    friend inline CLogger &operator << ( CLogger &logger,const bool &bEnd )
    {
        if ( bEnd )
            return logger;

        logger.close_log();

        return logger;
    }

    friend inline CLogger &operator << ( CLogger &logger,const char *buff )
    {
        logger.write_log( buff );
        return logger;
    }

    friend inline CLogger &operator << ( CLogger &logger,const string &buff )
    {
        logger.write_log( buff );
        return logger;
    }

    const string set_log_file_path( const string _path );
    const string get_log_file_path();

    bool write_log( const char *buff );
    bool write_log( const string buff );
    void flush();
    bool open_log();
    bool close_log();
    bool close_file();

private:
    string m_file_path;
    ofstream m_file;
};

#endif // CLOGGER_H
