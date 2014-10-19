#include "CMessageQueue.h"
#include "gslog.h"

CMessageQueue *CMessageQueue::m_pmsg = null;

CMessageQueue::CMessageQueue()
{
}

void CMessageQueue::resize_free_msg()
{
    CNetMessage *pmsg = null;
    int32 counter  = DEFAULT_MSG;
    while ( counter )
    {
        pmsg = new CNetMessage();
        m_free_msg.push_back( pmsg );

        counter --;
    }

    //如果需要释放deque的内存，shrink_to_fit()或参考swap()或由STL到合适的时候释放
}

void CMessageQueue::uninstance()
{
    //clear、erase不会自动调元素的析构函数
    //这里不会释放deque的内存,虽然它可能占用了大量内存

    CNetMessage *pmsg = null;

    deque<CNetMessage *>::iterator itr = m_free_msg.begin();
    while ( itr != m_free_msg.end() )
    {
        pmsg = *itr;

        delete pmsg;

        itr ++;
    }
    m_free_msg.clear();

    itr = m_write_cache_msg.begin();
    while ( itr != m_free_msg.end() )
    {
        pmsg = *itr;
        delete pmsg;
        itr ++;
    }
    m_write_cache_msg.clear();
}

void CMessageQueue::add_free_msg(CNetMessage *msg)
{
    if ( !msg )
        return;

    if ( m_free_msg.size() > MAX_FREE_MSG )
    {
        GWARNING() << "over max free msg occured\n";

        delete msg;
        return;
    }

    m_free_msg.push_back( msg );
}

void CMessageQueue::push_write_cache_msg(CNetMessage *msg)
{
    if ( !msg )
        return;

    if ( m_write_cache_msg.size() > MAX_CACHE_MSG )
    {
        GERROR() << "over max write cache msg,msg ignore\n";

        delete msg;
        return;
    }

    m_write_cache_msg.push_back( msg );
}

CNetMessage *CMessageQueue::get_free_msg()
{
    CNetMessage *pmsg = null;
    if ( m_free_msg.size() <= 0 )
        resize_free_msg();

    pmsg = m_free_msg.front();

    m_free_msg.pop_front();

    return pmsg;
}

deque<CNetMessage *> &CMessageQueue::get_write_cache_msg()
{
    return m_write_cache_msg;
}

void CMessageQueue::push_read_cache_msg(CNetMessage *msg)
{
    if ( null == msg )
        return;

    if ( m_read_cache_msg.size() > MAX_CACHE_MSG )
    {
        GERROR() << "over max read cache msg,msg ignore\n";

        delete msg;
        return;
    }

    m_read_cache_msg.push_back( msg );
}

deque<CNetMessage *> &CMessageQueue::get_read_cache_msg()
{
    return m_read_cache_msg;
}
