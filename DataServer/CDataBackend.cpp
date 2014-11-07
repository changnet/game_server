#include "CDataBackend.h"

#include <unistd.h>

CDataBackend::CDataBackend()
{
}

void CDataBackend::start()
{
    std::cout << "i am Data server,run..." << std::endl;
    //shm标识为O_WRONLY会导致mmap失败
    m_log_worker.init( O_CREAT | O_EXCL | O_RDWR,O_CREAT | O_EXCL,PROT_WRITE,S_IWUSR  );
    //m_log_worker.run_log_engine();
    m_log_worker.uninit();
}
