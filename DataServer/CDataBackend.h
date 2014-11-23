#ifndef CDATABACKEND_H
#define CDATABACKEND_H

#define DATABACKEND_TIME    0.005    //后台处理速度

#include "CLogWorker.h"
#include <ev++.h>

class CDataBackend
{
public:
    CDataBackend();

    void start();

private:  //function
    void backend( ev::timer &w,int32 revents );

private:
    CLogWorker m_log_worker;
    struct ev_loop *loop;
    ev::timer m_loop_timer;
    uint32 m_counter;
};

#endif // CDATABACKEND_H
