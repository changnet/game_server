/*
 * 包含读写缓冲区的共享内存
 *
 */

#ifndef CRWSHAREDMEMORY_H
#define CRWSHAREDMEMORY_H

#include "CSharedMemory.h"

class CRWSharedMemory : public CSharedMemory
{
public:
    CRWSharedMemory();

    bool map_shm( int32 type );

private:
    void *m_read_buff;
    void *m_write_buff;
};

#endif // CRWSHAREDMEMORY_H
