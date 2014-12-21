/*
 *
 * 仅读或写的共享内存缓冲区 uint32+数据区
 *
 */

#ifndef CRORWSHAREDMEMORY_H
#define CRORWSHAREDMEMORY_H

#include "CSharedMemory.h"

class CRorWSharedMemory : public CSharedMemory
{
public:
    CRorWSharedMemory();

    void zero_data_length();
    bool map_shm(int32 prot);
    bool unmap_shm();
    char *get_shm_data_buff();
    uint32 get_cache_length();
    bool write_buff( const void *src,uint32 length );

private:
    char   *m_shm_buff;
    uint32 *m_p_length;
    char   *m_p_data;
    uint32 m_data_size;
};

#endif // CRORWSHAREDMEMORY_H
