#ifndef CBACKEND_H
#define CBACKEND_H

#include "CUtility.h"
#include <ev++.h>

/**
 * @brief The CBackend class
 * 后台工作基类，为各个进程后台进程提供共同的功能
 * 包括初始化时间
 * 退出时销毁日志、工具类等对象内存...
 */
class CBackend
{
public:
    explicit CBackend();
    ~CBackend();
    
    void backend();

    virtual void start()   = 0;    /* 开始进入后台事件循环 */
    virtual void stop()    = 0;    /* 停止事件循环,作上层资源清理工作 */
    void abort_backend();                  /* 紧急中止，发生严重错误时调用，不存数据，不释放资源 */
    virtual void on_exit() = 0;    /* 退出时必须调用的接口 */

    bool signal_watch();           /* 捕捉系统信号 */
    void signal_handler( int signum );  /* 信号处理函数 */
protected:
    CUtility *m_utility;
    struct ev_loop *loop;
};

#endif // CBACKEND_H
