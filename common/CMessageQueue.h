#ifndef CMESSAGEQUEUE_H
#define CMESSAGEQUEUE_H

#include <deque>

#include "CNetMessage.h"

#define DEFAULT_MSG   1024
#define MAX_FREE_MSG  10240   //允许最多空闲消息
#define MAX_CACHE_MSG 2048    //最多缓存消息数

using std::deque;

class CMessageQueue
{
public:
    static CMessageQueue *instance()
    {
        if ( null == m_pmsg )
        {
            m_pmsg = new CMessageQueue();
            m_pmsg->resize_free_msg();
        }

        return m_pmsg;
    }

    void uninstance();

    CNetMessage *get_free_msg();
    void add_free_msg(CNetMessage *msg);

    void push_write_cache_msg(CNetMessage *msg);
    deque<CNetMessage *> &get_write_cache_msg();

    void push_read_cache_msg(CNetMessage *msg);
    deque<CNetMessage *> &get_read_cache_msg();

private:
    CMessageQueue();
    ~CMessageQueue();
    void resize_free_msg();

private:
    deque<CNetMessage *> m_free_msg;    //空闲消息结构体
    deque<CNetMessage *> m_write_cache_msg;   //缓存的消息，等待写入共享缓冲区
    deque<CNetMessage *> m_read_cache_msg;    //从缓冲区读出来，等待处理的消息

    static CMessageQueue *m_pmsg;
};

#endif // CMESSAGEQUEUE_H
