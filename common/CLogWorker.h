#ifndef CLOGWORKER_H
#define CLOGWORKER_H

#include "CSeamphoreLock.h"
#include "CRorWSharedMemory.h"
#include "CUtility.h"
#include "CLogger.h"

#define CONFIG_LENGTH        128           //小于 PATH_MAX limits.h
#define IPC_LOG_HEAD         "gs-log-%s"   //内存共享文件名
#define TRY_LOG_INTERVAL     3             //多久超时写入log
#define FORCE_LOG_TIMES      5             //强制写入日志时次数
#define FORCE_LOCK_TIME      2             //强制写入等待最长时间

class CLogWorker
{
public:
    CLogWorker();

    bool init(int32 shm_flag, int32 shm_mode, int32 sem_flag, int32 sem_mode, int32 map_prot);
    bool run_log_engine();
    bool wait(uint32 nsec, uint32 sec);
    bool unwait();
    void uninit();
    bool flush_log();
private:
    void flush();
    bool try_flush();
    bool force_flush();

private:
    CSeamphoreLock m_sem_lock;
    CRorWSharedMemory m_shm;

    int32 m_pid;
    uint8 m_fail_times;   //尝试写入失败次数
    time_t m_last;        //上一次写入缓冲区时间，超时将强制写入

};

#endif // CLOGWORKER_H
