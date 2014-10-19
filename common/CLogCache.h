#ifndef CLOGCACHE_H
#define CLOGCACHE_H

#include <map>
#include <deque>

#include "CPacket.h"

using std::map;
using std::deque;

#define PACKET_SIZE    4096

class CLogCache
{
public:
    CLogCache();

    void read_log();
    void write_log();
    void add_log(const char *path,const char *content);

private:    //function
    void resize_free_packet();
private:
    deque<CPacket*> m_free_packet;

    map<string,deque<CPacket*>> m_cache_packet;
};

#endif // CLOGCACHE_H
