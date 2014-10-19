#include "CErrorLog.h"
#include <cstdio>    //for stderr

CErrorLog *CErrorLog::m_plogger = null;

CErrorLog::CErrorLog(const string _path)
{
    m_file_path = _path;

    std::freopen ( m_file_path.c_str(),FILEFLAG,stderr );  //redirect cerr
}

CErrorLog::~CErrorLog()
{
}

void CErrorLog::uninstance()
{
    if ( m_plogger )
        delete m_plogger;

    m_plogger = null;
}


