#ifndef CLOGWORKER_H
#define CLOGWORKER_H

#include "CSeamphoreLock.h"
#include "CRorWSharedMemory.h"

#define CONFIG_LENGTH    128  //小于 PATH_MAX limits.h
#define IPC_LOG_HEAD     "gs-log-%s"

class CLogWorker
{
public:
    CLogWorker();

    bool init(int32 shm_flag, int32 sem_flag, int32 prot, int32 mode);
    bool run_log_engine();
    void uninit();

private:
    CSeamphoreLock m_sem_lock;
    CRorWSharedMemory m_shm;

    int32 m_pid;

};

#endif // CLOGWORKER_H
