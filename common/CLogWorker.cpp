#include <unistd.h>
#include <cstdio>     /* for snprintf */

#include "CLogWorker.h"
#include "CServerSetting.h"

CLogWorker::CLogWorker()
{
    m_last       = 0;
    m_fail_times = 0;
}

void CLogWorker::uninit()
{
    if ( !m_shm.unmap_shm() )  //必须先unmap才能关闭a共享内存
    {
        GERROR() << "fail to munmap shm:" << strerror(errno) << std::endl;
    }

    m_shm.close_shm();
    if ( m_sem_lock.close() < 0 )
    {
        GERROR() << "fail to close sem lock:" << strerror(errno) << std::endl;
    }
}

/**
 * @brief CLogWorker::init
 * @return
 *
 * master O_CREAT | O_EXCL,O_CREAT | O_EXCL,PROT_WRITE,S_IWUSR
 * 本函数只能初始化一次
 */
bool CLogWorker::init(int32 shm_flag,int32 shm_mode, int32 sem_flag,int32 sem_mode,int32 map_prot)
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

    /* O_CREAT | O_EXCL防止错误使用了其他进程的shm
     * 这里初始化信号量为0，让本进程无法锁定。log进程初始化时+1会解锁
     */
    if ( !m_sem_lock.open( tmp_buff,sem_flag,sem_mode,0 ) )
    {
        GFATAL() << "fail to init log sem:" << strerror( errno ) << std::endl;
        return false;
    }

    if ( snprintf(tmp_buff,CONFIG_LENGTH,IPC_LOG_HEAD,p_serverid) < 0 )
    {
        GFATAL() << "snprintf error while set sem id\n";
        return false;
    }
    if ( !m_shm.open_shm( tmp_buff,shm_flag,shm_mode ) )
    {
        uninit();

        GFATAL() << "fail to open log shm:" << strerror( errno ) << std::endl;
        return false;
    }

    if ( !m_shm.map_shm( map_prot ) )
    {
        uninit();

        GFATAL() << "fail to map shm buff:" << strerror( errno ) << std::endl;
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

/**
 * @brief CLogWorker::wait
 * @param nsec
 * @param sec
 * @return
 * 锁初始化为1，等子进程解锁后为0才能OK
 */
bool CLogWorker::wait(uint32 nsec,uint32 sec)
{
    int ret = m_sem_lock.time_lock( nsec,sec );
    if ( -1 == ret )
        return false;

    ret = m_sem_lock.unlock();
    if ( -1 == ret )
        return false;

    return true;
}

bool CLogWorker::unwait()
{
    if ( m_sem_lock.unlock() < 0 )
        return false;

    return true;
}

/**
 * @brief CLogWorker::flush
 * 将缓冲的log写入共享内存
 */
void CLogWorker::flush()
{
    bool ret = false;
    CLogger *plog = CLogger::instance();

    deque<CLogMessage*> &cache_msg = plog->get_cache_msg();
    deque<CLogMessage*>::iterator itr = cache_msg.begin();

    while ( itr != cache_msg.end() )
    {
        CLogMessage *pmsg = *itr;

        /* 写入日志到缓冲区 */
        ret = m_shm.write_buff( pmsg->get_buff_ptr(),pmsg->get_length() );
        if ( !ret )
        {
            GFATAL() << "log shm buff overflow,log abort\n";
            break;
        }

        plog->add_free_msg( pmsg );  //写入了就交给空闲处理

        itr ++;  //放到free队列后才++
    }

    cache_msg.clear();

    m_last = CUtility::instance()->time();
    m_fail_times = 0;
}

/**
 * @brief CLogWorker::try_flush
 * 尝试写入日志，如果发生竞争，则暂时不写入
 */
bool CLogWorker::try_flush()
{
    int32 ret = m_sem_lock.try_lock();
    if ( -1 == ret )
    {
        if ( errno != EAGAIN )
        {
            GERROR() << "try flush log,lock fail:" << strerror( errno ) << std::endl;
            return false;
        }

        m_fail_times ++;  //记录失败次数
        return false;
    }

    flush();

    m_sem_lock.unlock();

    return true;
}

/**
 * @brief CLogWorker::force_flush
 * 强制写入日志，如果有竞争，将阻塞，适用于关服、缓冲区已满
 */
bool CLogWorker::force_flush()
{
    int32 ret = m_sem_lock.time_lock( 0,FORCE_LOCK_TIME );
    if ( -1 == ret )
    {
        GERROR() << "force flush log,lock fail,log maybe lost:" << strerror( errno ) << std::endl;
        return false;
    }

    flush();

    m_sem_lock.unlock();

    return true;
}

/**
 * @brief CLogWorker::flush_log
 * @return
 * 1 超时尝试写入
 * 2 多次尝试后超时强制写入
 * 3 缓冲区满强制写入
 */
bool CLogWorker::flush_log()
{
    CLogger *ploger = CLogger::instance();
    if ( ploger->get_cache_size() <= 0 )  //无日志，无需写入.也算处理成功
        return true;

    if ( ploger->is_cache_full() )  //缓冲区已满，强制写入
        return force_flush();

    if ( m_fail_times >= FORCE_LOG_TIMES )  //try多次后仍无法写入，需要强制写入
    {
        return force_flush();
    }

    int32 interval = CUtility::instance()->time() - m_last;
    if ( interval < TRY_LOG_INTERVAL )    //缓冲未超时，先不写入
        return false;

    return try_flush();  //尝试写入
}

/**
 * @brief CLogWorker::read_log_from_shm
 * @return
 * 从共享内存读取日志
 * 1 超时尝试读取
 * 2 多次失败后强制读取
 */
bool CLogWorker::read_shm_log()
{
    if ( m_fail_times >= FORCE_LOG_TIMES )  //try多次后仍无法写入，需要强制写入
    {
        return force_read_log_from_shm();
    }

    int32 interval = CUtility::instance()->time() - m_last;
    if ( interval < TRY_LOG_INTERVAL )    //缓冲未超时，先不写入
        return false;

    return try_read_log_from_shm();
}

/**
 * @brief CLogWorker::read_log_from_shm
 * 从共享内存读取日志
 */
void CLogWorker::read_log_from_shm()
{
    char *pbuff = m_shm.get_shm_data_buff();
    uint32 length = m_shm.get_cache_length();
    uint32 read_length = 0;
    char path[128];
    char content[128];

    while ( read_length < length )
    {
        //TODO  这里数据应写回CLogMessage来缓存，但原operator << 设计有问题，待重新处理

        /* 读取协议长度 */
        strhead *pstr_len = reinterpret_cast<strhead *>(pbuff+read_length);
        read_length += sizeof(strhead);

        /* 读取路径长度 */
        pstr_len =reinterpret_cast<strhead *>(pbuff+read_length);
        read_length += sizeof(strhead);

        memcpy( path,pbuff+read_length,*pstr_len );
        read_length += *pstr_len;

        /* 读取内容长度 */
        pstr_len =reinterpret_cast<strhead *>(pbuff+read_length);
        read_length += sizeof(strhead);

        memcpy( content,pbuff+read_length,*pstr_len );
        read_length += *pstr_len;

        std::cout << "file:" << path << "  content:" << content << std::endl;
    }

    m_shm.zero_data_length();
}

/**
 * @brief CLogWorker::try_read_log_from_shm
 * @return  成功true,失败false
 * 尝试锁定共享内存读取日志
 */
bool CLogWorker::try_read_log_from_shm()
{
    int32 ret = m_sem_lock.try_lock();
    if ( -1 == ret )
    {
        if ( errno != EAGAIN )
        {
            GERROR() << "try_read_log_from_shm,lock fail:" << strerror( errno ) << std::endl;
            return false;
        }

        m_fail_times ++;  //记录失败次数
        return false;
    }

    read_log_from_shm();

    m_sem_lock.unlock();

    return true;
}

/**
 * @brief CLogWorker::froce_read_log_from_shm
 * @return 成功true,失败false
 */
bool CLogWorker::force_read_log_from_shm()
{
    int32 ret = m_sem_lock.time_lock( 0,FORCE_LOCK_TIME );
    if ( -1 == ret )
    {
        GERROR() << "froce_read_log_from_shm,lock fail,log maybe lost:" << strerror( errno ) << std::endl;
        return false;
    }

    read_log_from_shm();

    m_sem_lock.unlock();

    return true;
}
