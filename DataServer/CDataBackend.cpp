#include "CDataBackend.h"

#include <unistd.h>

#include "CUtility.h"

CDataBackend::CDataBackend()
{
    m_counter = 0;
    loop = EV_DEFAULT;
}

void CDataBackend::start()
{
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

    m_loop_timer.set< CDataBackend,&CDataBackend::backend >(this);
    m_loop_timer.set( DATABACKEND_TIME,DATABACKEND_TIME );
    m_loop_timer.start();

    ev_run( loop,0 );

    m_log_worker.uninit();
}

void CDataBackend::backend(ev::timer &w, int32 revents)
{
    if ( EV_ERROR & revents )
    {
        GFATAL() << "backend error:" << strerror(errno) << "\n";
        w.stop();
    }

    CBackend::backend();


    GINFO( "test.log" ) << "update ...." << ++m_counter << std::endl;
    m_log_worker.flush_log();
}
