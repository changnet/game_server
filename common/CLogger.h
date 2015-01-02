#ifndef CLOGGER_H
#define CLOGGER_H


#include <map>
#include <deque>

#include "CLogMessage.h"

using std::deque;

#define LOG_MSG_SIZE    4096

class CLogger
{
public:
    static CLogger *instance();
    static void uninstance();

    bool is_cache_full();
    uint32 get_cache_size();

    void add_free_msg( CLogMessage *pmsg );
    CLogMessage &message(const char *path);
    deque<CLogMessage*> &get_cache_msg();

private:    //function
    CLogger();
    ~CLogger();
    void resize_free_packet();
private:
    deque<CLogMessage*> m_free_msg;

    deque<CLogMessage*> m_cache_msg;

    static CLogger *m_p_logger;
};


#endif // CLOGGER_H
