/*
 * 网络数据包存储类
 *
 */

#ifndef CPACKET_H
#define CPACKET_H

#include <cstring>  /* for memset */
#include "gstypes.h"

#define MSG_LENGTH    32768   //长度 32*1024

class CPacket
{
public:
    CPacket();

    void zero();
    void truncate();
    char *get_buff();
    uint32 get_length();
    uint32 set_length(uint32 length);
    uint32 add_length(uint32 length);

    char buff[MSG_LENGTH]; /* 暴露出来方便将socket中的数据直接拷贝进缓冲区 */

private:

    uint32 m_buff_length;
    uint32 m_current_pos;

};

#endif // CPACKET_H
