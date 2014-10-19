#ifndef CSEAMPHORELOCK_H
#define CSEAMPHORELOCK_H

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include "gstypes.h"
#include "gslog.h"

#define SEM_VALUE    1  //The value argument specifies the initial value for the new semaphore

class CSeamphoreLock
{
public:
    CSeamphoreLock();
    int32 lock();
    int32 try_lock();
    int32 time_lock( int32 nano_sec,int32 sec = 0 );  //Nanoseconds [0 .. 999999999]
    int32 unlock();
    int32 get_value();
    int32 unlink();
    bool open( const char *name,int32 oflag,mode_t mode, uint32 value );
    int32 close();
private:
    sem_t *m_psem;
    const char *m_sem_name;
};

#endif // CSEAMPHORELOCK_H
