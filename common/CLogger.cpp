#include "CLogger.h"
#include "gslog.h"

CLogger *CLogger::m_p_logger = null;

CLogger::CLogger()
{
    resize_free_packet();  //预分配空间
}

/* 释放内存 */
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

/* 重新分配空闲包 */
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

/* 获取一个消息包对象，并设置log文件名 */
CLogMessage &CLogger::message(const char *path)
{
    if ( m_free_msg.empty() )
    {
        resize_free_packet();
        GWARNING() << "log message deque resize occure\n";
    }

    CLogMessage *p = m_free_msg.front();
    m_free_msg.pop_front();

    p->zero();    //must clear old data
    p->init( path );
    m_cache_msg.push_back( p );

    return *p;
}

CLogger *CLogger::instance()
{
    if ( null == m_p_logger )
        m_p_logger = new CLogger();

    return m_p_logger;
}

void CLogger::uninstance()
{
    if ( m_p_logger )
        delete m_p_logger;

    m_p_logger = null;
}

/**
 * @brief CLogger::is_cache_full
 * @return
 * 缓存的日志是否已满，满了则需要强制写入缓冲区
 */
bool CLogger::is_cache_full()
{
    return m_cache_msg.size() > LOG_MSG_SIZE ? true : false;
}

/**
 * @brief CLogger::get_cache_size
 * @return
 * 获取当前缓冲的日志消息数量
 */
uint32 CLogger::get_cache_size()
{
    return m_cache_msg.size();
}

/* 获取缓冲的log队列 */
deque<CLogMessage*> &CLogger::get_cache_msg()
{
    return m_cache_msg;
}

void CLogger::add_free_msg(CLogMessage *pmsg)
{
    assert( null != pmsg );

    m_free_msg.push_back( pmsg );
}
