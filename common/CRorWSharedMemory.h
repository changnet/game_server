/*
 *
 * 仅读或写的共享内存缓冲区
 *
 */

#ifndef CRORWSHAREDMEMORY_H
#define CRORWSHAREDMEMORY_H

#include "CSharedMemory.h"

class CRorWSharedMemory : public CSharedMemory
{
public:
    CRorWSharedMemory();

    bool map_shm(ESharedMemoryType type);
    void *get_shm_buff();

private:
    void *m_shm_buff;
};

#endif // CRORWSHAREDMEMORY_H
