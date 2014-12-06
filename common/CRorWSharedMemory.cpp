#include "CRorWSharedMemory.h"

CRorWSharedMemory::CRorWSharedMemory()
{
    m_shm_buff  = null;
    m_p_length  = null;
    m_p_data    = null;
    m_data_size = 0;
}

/* 获取数组区指针 */
void *CRorWSharedMemory::get_shm_data_buff()
{
    return m_p_data;
}

/* 获取已使用的缓冲区长度 */
uint32 *CRorWSharedMemory::get_cache_length()
{
    return m_p_length;
}

bool CRorWSharedMemory::map_shm(int32 prot)
{
    int32 shm_fd = get_file_description();

    m_shm_buff = mmap( null,SHM_PAGE_SIZE,prot,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );

    if ( MAP_FAILED == m_shm_buff )
        return false;

    m_p_length  = new( m_shm_buff ) uint32;
    m_p_data    = static_cast<char *>(m_shm_buff) + sizeof( *m_p_length );
    m_data_size = SHM_PAGE_SIZE - sizeof( *m_p_length );

    return true;
}

bool CRorWSharedMemory::unmap_shm()
{
    if ( null == m_shm_buff || MAP_FAILED == m_shm_buff ) //可能未初始化，可能初始化失败
        return true;

    int32 ret = munmap( m_shm_buff,SHM_PAGE_SIZE );
    if ( ret < 0 )
        return false;

    m_p_length  = null;
    m_p_data    = null;
    m_data_size = 0;

    return true;  //ret = 0
}

/* 写入内容到共享内存 */
bool CRorWSharedMemory::write_buff(const void *src, uint32 length)
{
    if ( *m_p_length + length > m_data_size )
        return false;

    memcpy( m_p_data+*m_p_length,src,length );
    *m_p_length += length;

    return true;
}
