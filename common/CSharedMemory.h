#ifndef CSHAREDMEMORY_H
#define CSHAREDMEMORY_H

/* 先shm_open一块可RW的内存。然后一分为二，分为R和W的两块内存。
 * 每块R（或W）的内存又划分为系统共享内存和用户内存
 * 对于系统内存，里面的消息为进程通信，由进程自己本身处理
 * 如：新用户接入，socket断开
 * 对于用户内存，NET引擎则只是转发
 */

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>          /* For sysconf */

#include "gstypes.h"

#define SHM_PAGE_SIZE    67108864          //64M,必须是_SC_PAGE_SIZE的倍数
#define SHM_RW_SIZE      SHM_PAGE_SIZE/2   //读写BUFF一分为2

enum ESharedMemoryType
{
    ESHMT_MASTER,
    ESHMT_SLAVE
};

class CSharedMemory
{
public:
    CSharedMemory();

    bool open_shm( const char *name, int32 oflag, mode_t mode );
    void close_shm();
    virtual bool map_shm( ESharedMemoryType type ) = 0;

    int32 get_file_description();
    const char *get_shm_name();

private:
    int32 m_shm_fd;
    const char *m_shm_name;
};

#endif // CSHAREDMEMORY_H
