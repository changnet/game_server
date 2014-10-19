#include "CRorWSharedMemory.h"

CRorWSharedMemory::CRorWSharedMemory()
{
    m_shm_buff = null;
}

void *CRorWSharedMemory::get_shm_buff()
{
    return m_shm_buff;
}

bool CRorWSharedMemory::map_shm(ESharedMemoryType type)
{
    int32 shm_fd = get_file_description();
    if ( ESHMT_MASTER == type ) //主进程
        m_shm_buff = mmap( null,SHM_PAGE_SIZE,PROT_READ,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );
    else
        m_shm_buff = mmap( null,SHM_PAGE_SIZE,PROT_WRITE,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );

    if ( MAP_FAILED == m_shm_buff )
        return false;

    return true;
}
