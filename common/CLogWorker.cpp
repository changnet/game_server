#include <unistd.h>
#include <cstdio>     /* for snprintf */

#include "CLogWorker.h"
#include "CServerSetting.h"

CLogWorker::CLogWorker()
{
}

void CLogWorker::uninit()
{
}

/**
 * @brief CLogWorker::init
 * @return
 *
 * master O_CREAT | O_EXCL,O_CREAT | O_EXCL,PROT_WRITE
 * 本函数只能初始化一次
 */
bool CLogWorker::init(int32 shm_flag, int32 sem_flag, int32 prot)
{
    char tmp_buff[CONFIG_LENGTH];

    const char *p_serverid     = CServerSetting::instance().get_server_id();

    if ( null == p_serverid )
    {
        GFATAL() << "can not get server id config\n";
        return false;
    }

    if ( snprintf(tmp_buff,CONFIG_LENGTH,IPC_LOG_HEAD,p_serverid) < 0 )
    {
        GFATAL() << "snprintf error while set sem id\n";
        return false;
    }

    //O_CREAT | O_EXCL防止错误使用了其他进程的shm
    if ( !m_sem_lock.open( tmp_buff,shm_flag,S_IWUSR,0 ) )
    {
        GFATAL() << "fail to init log sem:" << strerror( errno ) << std::endl;
        return false;
    }

    if ( !m_shm.open_shm( tmp_buff,sem_flag,S_IWUSR ) )
    {
        uninit();

        GFATAL() << "fail to open log shm:" << strerror( errno ) << std::endl;
        return false;
    }

    if ( !m_shm.map_shm( prot ) )
    {
        uninit();

        GFATAL() << "fail to map shm buff\n" << strerror( errno ) << std::endl;
        return false;
    }

    return true;
}

bool CLogWorker::run_log_engine()
{
    const char *p_serverid     = CServerSetting::instance().get_server_id();

    if ( null == p_serverid )
    {
        GFATAL() << "can not get server id config\n";
        return false;
    }

    char engine_name[CONFIG_LENGTH];
    char server_id[CONFIG_LENGTH];

    if ( snprintf(engine_name,CONFIG_LENGTH,"%s",CServerSetting::instance().get_log_engine_name()) < 0
         || snprintf(server_id,CONFIG_LENGTH,"%s",p_serverid) < 0
         )
    {
        GFATAL() << "snprintf error while set sem id\n";
        return false;
    }

    const char *p_engine_path  = CServerSetting::instance().get_log_engine_path();

    char* const argv[] = { engine_name,server_id,null };

    int32 pid = fork();
    if ( 0 == pid )  //子进程
    {
        int32 ret = execv( p_engine_path,argv );
        if ( ret < 0 ) //error
        {
            GFATAL() << "exec log engine fail:" << strerror( errno ) << std::endl;
            return false;
        }

        return false;//never run to here
    }
    else if ( pid > 0 )  //父进程
    {
        m_pid = pid;  //记录子进程的pid
        return true;
    }
    else
    {
        uninit();
        return false;
    }

    return false;  //never run to hero
}

