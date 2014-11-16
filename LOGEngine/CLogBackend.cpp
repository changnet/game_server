#include "CLogBackend.h"

CLogBackend::CLogBackend()
{
    loop = EV_DEFAULT;
}

void CLogBackend::start_work()
{
    m_log_worker.init( O_CREAT | O_RDWR,S_IRUSR,O_CREAT | O_RDWR,S_IWUSR | S_IRUSR,PROT_WRITE );
    m_log_worker.unwait();

    m_log_timer.set< CLogBackend,&CLogBackend::backend >(this);
    m_log_timer.set( LOGBACKEND_TIME,LOGBACKEND_TIME );
    m_log_timer.start();

    ev_run( loop,0 );
}

void CLogBackend::end_work()
{
}

void CLogBackend::backend(ev::timer &w, int32 revents)
{
    if ( EV_ERROR & revents )
    {
        GFATAL() << "backend error:" << strerror(errno) << "\n";
        w.stop();
    }
}
