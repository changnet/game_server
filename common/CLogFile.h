#ifndef CLOGFILE_H
#define CLOGFILE_H

#include <iostream>
#include <fstream>
#include "gstype.h"

#define GERRORFILE    "error.log"
#define GFATALFILE    "fatal.log"
#define FILEFLAG      "ab+"


class CLogFile
{
public:
    CLogFile();

    static std::ostream &error();
    static std::ostream &fatal();
    CLogFile &log_file(const string &path);
    template<typename T>
    CLogFile &operator << ( T &t);
    void operator << ( std::ostream& (*pf)(std::ostream&) );

private:
    std::ofstream m_file;
    const string m_path;
};

#endif // CLOGFILE_H
