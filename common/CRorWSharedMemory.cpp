#include "CRorWSharedMemory.h"

CRorWSharedMemory::CRorWSharedMemory()
{
    m_shm_buff = null;
}

void *CRorWSharedMemory::get_shm_buff()
{
    return m_shm_buff;
}

bool CRorWSharedMemory::map_shm(int prot)
{
    int32 shm_fd = get_file_description();

    m_shm_buff = mmap( null,SHM_PAGE_SIZE,prot,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );

    if ( MAP_FAILED == m_shm_buff )
        return false;

    return true;
}

bool CRorWSharedMemory::unmap_shm()
{
    if ( null == m_shm_buff || MAP_FAILED == m_shm_buff ) //可能未初始化，可能初始化失败
        return true;

    int32 ret = munmap( m_shm_buff,SHM_PAGE_SIZE );
    if ( ret < 0 )
        return false;

    return true;  //ret = 0
}
