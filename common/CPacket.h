/*
 * 网络数据包存储类
 *
 */

#ifndef CPACKET_H
#define CPACKET_H

#include "gstypes.h"

#define PACKET_LENGTH    32768   //长度 32*1024

class CPacket
{
public:
    CPacket();

    void zero();
    void truncate();
    uint32 get_length();
    uint32 get_packet_length();
    char *get_buff_ptr();
    bool fill_buff( void *p,uint32 length );

protected:
    char m_buff[PACKET_LENGTH];
    msghead *m_p_length;
};

#endif // CPACKET_H
