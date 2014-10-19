#include "CSharedMemory.h"

CSharedMemory::CSharedMemory()
{
    m_shm_fd = 0;
    m_shm_name    = null;
}

void CSharedMemory::close_shm()
{
    if ( m_shm_fd )
        close( m_shm_fd );

    if ( m_shm_name )
        shm_unlink( m_shm_name );

    m_shm_fd = 0;
    m_shm_name = null;

}

bool CSharedMemory::open_shm( const char *name, int32 oflag, mode_t mode )
{
    m_shm_fd = shm_open( name,oflag,mode );
    if ( -1 == m_shm_fd )
    {
        return false;
    }

    m_shm_name = name;

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

