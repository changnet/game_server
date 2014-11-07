#include "CSeamphoreLock.h"

CSeamphoreLock::CSeamphoreLock()
{
    m_sem_name[0] = '\0';
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
    if ( snprintf( m_sem_name,SEM_NAME_LEN,"%s",name ) < 0 )
        return false;

    m_psem = sem_open( m_sem_name,oflag,mode,value );
    if ( SEM_FAILED == m_psem )  //error
    {
        m_psem = null;
        return false;
    }

    return true;
}

/**
 * @brief CSeamphoreLock::lock
 * @return
 * sem_post函数的作用是给信号量的值加上一个“1”，它是一个“原子操作”sem_wait函数也是一个原子操作，它的作用
 * 是从信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。也就是说，如果你对一个值
 * 为2的信号量调用sem_wait(),线程将会继续执行，信号量的值将减到1。如果对一个值为0的信号量调用sem_wait()，
 * 这个函数就会地等待直到有其它线程增加了这个值使它不再是0为止。
 */
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
bool CSeamphoreLock::get_value( int32 sval )
{
    if ( sem_getvalue( m_psem,&sval ) < 0 )
        return false;

    return true;
}


int32 CSeamphoreLock::close()
{
    if ( !m_psem )
        return 0;

    if ( sem_close( m_psem ) < 0 || sem_unlink( m_sem_name ) < 0 )
        return -1;

    m_psem = null;

    return 0;
}
