#ifndef CSEAMPHORELOCK_H
#define CSEAMPHORELOCK_H

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "gstypes.h"
#include "gslog.h"

#define SEM_VALUE    1  //The value argument specifies the initial value for the new semaphore
#define SEM_NAME_LEN 128

class CSeamphoreLock
{
public:
    CSeamphoreLock();
    int32 lock();
    int32 try_lock();
    int32 time_lock( int32 nano_sec,int32 sec = 0 );  //Nanoseconds [0 .. 999999999]
    int32 unlock();
    bool get_value(int32 sval);
    bool open( const char *name,int32 oflag,mode_t mode, uint32 value );
    int32 close();
private:
    sem_t *m_psem;
    char m_sem_name[SEM_NAME_LEN];
};

#endif // CSEAMPHORELOCK_H
