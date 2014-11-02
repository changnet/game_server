#include "CLogger.h"
#include "gslog.h"

CLogger *CLogger::m_p_logger = null;

CLogger::CLogger()
{
    resize_free_packet();  //预分配空间
}

CLogger::~CLogger()
{
    deque<CLogMessage*>::iterator itr = m_free_msg.begin();
    while ( itr != m_free_msg.end() )
    {
        delete *itr;
        itr ++;
    }

    deque<CLogMessage*>::iterator itr_cache = m_cache_msg.begin();
    while ( itr_cache != m_cache_msg.end() )
    {
        delete *itr_cache;
        itr_cache ++;
    }
}

void CLogger::resize_free_packet()
{
    CLogMessage *p = null;
    int32 size = LOG_MSG_SIZE;

    while ( size > 0 )
    {
        p = new CLogMessage();
        p->zero();

        m_free_msg.push_back( p );

        size --;
    }
}

CLogMessage &CLogger::message(const char *path)
{
    if ( m_free_msg.empty() )
    {
        resize_free_packet();
        GWARNING() << "log message deque resize occure\n";
    }

    CLogMessage *p = m_free_msg.front();
    m_free_msg.pop_front();

    p->set_path( path );
    m_cache_msg.push_back( p );

    return *p;
}

void CLogger::read_log_from_shm()
{
}

void CLogger::write_log_to_shm()
{
    deque<CLogMessage*>::iterator itr = m_cache_msg.begin();
    while ( itr != m_cache_msg.end() )
    {
        std::cout << "file:" << (*itr)->get_path() << " content:" << (*itr)->buff << std::endl;
        itr ++;

        m_free_msg.push_back( *itr );  //写入了就交给空闲处理
    }

    m_cache_msg.clear();
}

CLogger &CLogger::instance()
{
    if ( null == m_p_logger )
        m_p_logger = new CLogger();

    return *m_p_logger;
}

void CLogger::uninstance()
{
    if ( m_p_logger )
        delete m_p_logger;

    m_p_logger = null;
}
