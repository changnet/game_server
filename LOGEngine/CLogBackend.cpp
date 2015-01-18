#include "CLogBackend.h"

CLogBackend::CLogBackend()
{
    loop = EV_DEFAULT;
}

/**
 * @brief CLogBackend::start
 * 开始日志引擎后台事件循环
 */
void CLogBackend::start()
{
    m_log_worker.init( O_CREAT | O_RDWR,S_IRUSR,O_CREAT | O_RDWR,S_IWUSR | S_IRUSR,PROT_WRITE );
    m_log_worker.unwait();

    m_log_timer.set< CLogBackend,&CLogBackend::backend >(this);
    m_log_timer.set( LOGBACKEND_TIME,LOGBACKEND_TIME );
    m_log_timer.start();

    ev_run( loop,0 );
}

/**
 * @brief CLogBackend::stop
 * 终止日志引擎后台事件循环
 */
void CLogBackend::stop()
{
    on_exit();
}

/**
 * @brief CLogBackend::backend
 * @param w
 * @param revents
 *
 * 1.处理缓存的日志
 *    (1)日志超量需要写入文件
 *    (2)日志超时需要写入文件
 * 2.从共享内存读取日志,try_lock失败一定次数后强制阻塞等待锁定
 */
void CLogBackend::backend(ev::timer &w, int32 revents)
{
    if ( EV_ERROR & revents )
    {
        GERROR() << "backend error:" << strerror(errno) << "\n";
        w.stop();
    }

    CBackend::backend();

    m_log_worker.read_shm_log();

    //TODO 写入文件
}

/**
 * @brief CLogBackend::on_exit
 * 进程退出时清理资源
 */
void CLogBackend::on_exit()
{
    m_log_worker.uninit();
}
