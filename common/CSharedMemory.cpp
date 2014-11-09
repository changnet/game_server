#include "CSharedMemory.h"
#include <iostream>

CSharedMemory::CSharedMemory()
{
    m_shm_fd = -1;
    m_shm_name[0] = '\0';
}

void CSharedMemory::close_shm()
{
    if ( m_shm_fd < 0 )
        return;

    close( m_shm_fd );
    shm_unlink( m_shm_name );

    m_shm_fd = -1;
    m_shm_name[0] = '\0';

}

bool CSharedMemory::open_shm( const char *name, int32 oflag, mode_t mode )
{
    if ( snprintf(m_shm_name,SHM_NAME_LEN,"/%s",name) < 0 ) //must have /
        return false;

    //On success, shm_open() returns a nonnegative file descriptor.maybe 0 ??
    m_shm_fd = shm_open( m_shm_name,oflag,mode );
    if ( -1 == m_shm_fd )
    {
        return false;
    }

    struct stat st;
    if ( fstat( m_shm_fd,&st ) < 0 )
    {
        close_shm();
        return false;
    }

    if ( SHM_PAGE_SIZE == st.st_size )  //not a new create memory(SHM_PAGE_SIZE),no need to initially
        return true;

    //不允许两个进程间共享内存大小不一致
    if ( st.st_size > 0 )
    {
        close_shm();

        return false;
    }

    /* A  new  shared memory object initially has zero length—the size of the object can
     * be set using ftruncate(2)
     */
    if ( ftruncate( m_shm_fd,SHM_PAGE_SIZE ) < 0 )
    {
        close_shm();

        return false;
    }

    return true;
}

int32 CSharedMemory::get_file_description()
{
    return m_shm_fd;
}

const char *CSharedMemory::get_shm_name()
{
    return m_shm_name;
}

