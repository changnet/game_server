#include "CSeamphoreLock.h"

CSeamphoreLock::CSeamphoreLock()
{
    m_sem_name = null;
    m_psem = null;
}

/*
     about mode,see man 2 open:
              S_IRWXU  00700 user (file owner) has read, write and execute permission

              S_IRUSR  00400 user has read permission

              S_IWUSR  00200 user has write permission

              S_IXUSR  00100 user has execute permission

              S_IRWXG  00070 group has read, write and execute permission

              S_IRGRP  00040 group has read permission

              S_IWGRP  00020 group has write permission

              S_IXGRP  00010 group has execute permission

              S_IRWXO  00007 others have read, write and execute permission

              S_IROTH  00004 others have read permission

              S_IWOTH  00002 others have write permission

              S_IXOTH  00001 others have execute permission

  open( "/test",O_CREAT,S_IRWXU,0 )
*/
bool CSeamphoreLock::open( const char *name,int32 oflag,mode_t mode, uint32 value  )
{
    m_psem = sem_open( name,oflag,mode,value );
    if ( SEM_FAILED == m_psem )  //error
    {
        return false;
    }

    m_sem_name = name;
    return true;
}

int32 CSeamphoreLock::lock()
{
    return sem_wait( m_psem );
}

int32 CSeamphoreLock::try_lock()
{
    return sem_trywait( m_psem );
}

int32 CSeamphoreLock::time_lock( int32 nano_sec,int32 sec )
{
    struct timespec ts;
    ts.tv_sec  = sec;
    ts.tv_nsec = nano_sec;

    return sem_timedwait( m_psem,&ts );
}

int32 CSeamphoreLock::unlock()
{
    return sem_post( m_psem );
}

/* If one or more processes or threads are blocked waiting to lock the semaphore with sem_wait(3),
 * POSIX.1-2001 permits two possibilities for the value returned in sval: either 0 is returned; or
 * a negative number whose absolute value is the count of the number of processes and threads currently
 * blocked in sem_wait(3)
 * on error, -1 is returned and errno is set to indicate the error
 */
int32 CSeamphoreLock::get_value()
{
    int32 sval = 0;

    if ( sem_getvalue( m_psem,&sval ) < 0 )
        return -1;

    return sval;
}


int32 CSeamphoreLock::unlink()
{
    return sem_unlink( m_sem_name );
}

int32 CSeamphoreLock::close()
{
    return sem_close( m_psem );
}
