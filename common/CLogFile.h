#ifndef CLOGFILE_H
#define CLOGFILE_H

#include <iostream>
#include <fstream>
#include <cstdio>    /*  for stderr */
#include <cstring>   /*  for strerr */
#include <cerrno>    /*  for errno  */

#include "gstypes.h"

#define GERRORFILE    "error.log"
#define GFATALFILE    "fatal.log"
#define FILEFLAG      "ab+"


class CLogFile
{
public:
    static CLogFile &instance();
    static void unistance();

    static std::ostream &error();
    static std::ostream &fatal();
    CLogFile &log_file(const string &path,bool print = false);

    /*  C++ 模板不支持分离实现 */
    template<typename T>
    CLogFile &operator << ( T t )
    {
        if ( m_print )
            std::cout << t;

        m_file << t;

        if ( m_file.fail() || m_file.bad() )  //see http://www.cplusplus.com/reference/ostream/ostream/operator<</
        {
            error() << "write file<" << m_path << "> fail:" << strerror( errno ) << "\n";
        }

        return *this;
    }

    void operator << ( std::ostream& (*pf)(std::ostream&) );

private:
    CLogFile();

private:
    std::ofstream m_file;
    const string m_path;
    bool m_print;

    static CLogFile *m_plog_file;
};

#endif // CLOGFILE_H
