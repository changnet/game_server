#include "CLogBackend.h"

CLogBackend::CLogBackend()
{
    m_self_name  = null;
    m_center_pid = 0;
    m_server_id  = null;
}

void CLogBackend::set_self_name(const char *name)
{
    m_self_name = name;
}

const char *CLogBackend::get_self_name()
{
    return m_self_name;
}

void CLogBackend::set_server_id(const char *id)
{
    m_server_id = id;
}

const char *CLogBackend::get_server_id()
{
    return m_server_id;
}

void CLogBackend::set_center_pid(int32 pid)
{
    m_center_pid = pid;
}

int  CLogBackend::get_center_pid()
{
    return m_center_pid = 0;
}

void CLogBackend::start_work()
{
    m_log_worker.init( O_CREAT | O_RDWR,S_IRUSR,O_CREAT | O_RDWR,S_IWUSR | S_IRUSR ,PROT_WRITE );
    m_log_worker.unwait();
}

void CLogBackend::end_work()
{
}
