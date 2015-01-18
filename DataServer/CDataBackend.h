#ifndef CDATABACKEND_H
#define CDATABACKEND_H

#define DATABACKEND_TIME    0.005    //后台处理速度

#include "CBackend.h"
#include "CLogWorker.h"

class CDataBackend : public CBackend
{
public:
    CDataBackend();

    void start();
    void stop();
    void on_exit();

private:  //function
    void backend( ev::timer &w,int32 revents );

private:
    CLogWorker m_log_worker;
    ev::timer m_loop_timer;
    uint32 m_counter; /* TODO: 删除测试计数器 */
};

#endif // CDATABACKEND_H
