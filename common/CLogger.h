#ifndef CLOGGER_H
#define CLOGGER_H


#include <map>
#include <deque>

#include "CLogMessage.h"

using std::deque;

#define PACKET_SIZE    4096

class CLogger
{
public:
    CLogger();
    ~CLogger();

    void read_log_from_shm();
    void write_log_to_shm();
    CLogMessage &message(const char *path);

private:    //function
    void resize_free_packet();
private:
    deque<CLogMessage*> m_free_msg;

    deque<CLogMessage*> m_cache_msg;
};


#endif // CLOGGER_H
