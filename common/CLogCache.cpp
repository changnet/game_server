#include "CLogCache.h"

CLogCache::CLogCache()
{
    resize_free_packet();  //预分配空间
}

void CLogCache::resize_free_packet()
{
    CPacket *p = null;
    int32 size = PACKET_SIZE;

    while ( size > 0 )
    {
        p = new CPacket();
        p->zero();

        m_free_packet.push_back( p );
    }
}
