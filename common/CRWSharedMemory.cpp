#include "CRWSharedMemory.h"

CRWSharedMemory::CRWSharedMemory()
{
    m_read_buff  = null;
    m_write_buff = null;
}

/*
 * 对于主进程，第一段为R，第二段为W
 * 对于从进程，第一段为W，第二段为R
 * MAP_NORESERVE
 *     Do not reserve swap space for this mapping.  When swap space is reserved, one has the guarantee that it is  possible  to  modify  the
 *     mapping.   When  swap space is not reserved one might get SIGSEGV upon a write if no physical memory is available.  See also the dis‐
 *     cussion of the file /proc/sys/vm/overcommit_memory in proc(5).  In kernels before 2.6, this flag had effect only for private writable
 *     mappings.
 */
bool CRWSharedMemory::map_shm(ESharedMemoryType type)
{
    int32 shm_fd = get_file_description();
    if ( ESHMT_MASTER == type )
    {
        m_read_buff  = mmap( null,SHM_RW_SIZE,PROT_READ,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );
        m_write_buff = mmap( null,SHM_RW_SIZE,PROT_WRITE,MAP_SHARED | MAP_NORESERVE,shm_fd,SHM_RW_SIZE );
    }
    else
    {
        m_write_buff = mmap( null,SHM_RW_SIZE,PROT_WRITE,MAP_SHARED | MAP_NORESERVE,shm_fd,0 );
        m_read_buff  = mmap( null,SHM_RW_SIZE,PROT_READ,MAP_SHARED | MAP_NORESERVE,shm_fd,SHM_RW_SIZE );
    }

    //if you memcpy to a buff not PROT_WRITE,will crashed

    //On error, the value MAP_FAILED (that is, (void *) -1) is returned
    if ( MAP_FAILED == m_read_buff || MAP_FAILED == m_write_buff )
    {
        close_shm();
        return false;
    }

    return true;
}
