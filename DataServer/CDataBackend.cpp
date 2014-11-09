#include "CDataBackend.h"

#include <unistd.h>

CDataBackend::CDataBackend()
{
}

void CDataBackend::start()
{
    std::cout << "i am Data server,run..." << std::endl;
    /* shm需要S_IRUSR|S_IWUSR，因为这里是创建，本进程不需要读，其他进程需要。否则其他进程Permission denied
     * shm标识为O_WRONLY会导致mmap失败，因为需要读取一些信息，如大小
     */
    m_log_worker.init( O_CREAT | O_EXCL | O_RDWR,S_IRUSR|S_IWUSR ,
        O_CREAT | O_EXCL | O_RDWR,S_IWUSR | S_IRUSR ,PROT_WRITE );
    m_log_worker.run_log_engine();

    if ( m_log_worker.wait(2000,20) )
        std::cout << "lock init ok\n";
    else
        std::cout << "lock init timeout\n";

    m_log_worker.uninit();
}
