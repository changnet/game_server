#ifndef CLOGBACKEND_H
#define CLOGBACKEND_H

#include "gstypes.h"
#include "CLogWorker.h"
#include "ev++.h"

#define LOGBACKEND_TIME    0.05    //log进程运行速度

class CLogBackend
{
public:
    CLogBackend();

    void start_work();
    void end_work();

private:
    void backend( ev::timer &w, int32 revents );

private:
    CLogWorker m_log_worker;
    ev::timer m_log_timer;
    struct ev_loop *loop;
};

#endif // CLOGBACKEND_H
