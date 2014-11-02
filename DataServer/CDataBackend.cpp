#include "CDataBackend.h"

#include <unistd.h>

CDataBackend::CDataBackend()
{
}

void CDataBackend::start()
{
    std::cout << "i am Data server,run..." << std::endl;
    m_log_worker.run_log_engine();
}
