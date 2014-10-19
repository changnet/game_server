#include "CSingletonLog.h"

CSingletonLog *CSingletonLog::m_plogger = null;

CSingletonLog::CSingletonLog()
{
}

CSingletonLog::~CSingletonLog()
{
}

void CSingletonLog::uninstance()
{
    if ( m_plogger )
        delete m_plogger;

    m_plogger = null;
}
