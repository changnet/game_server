/*
 * 服务器消息类
 * 消息可分为服务器消息和用户消息
 * m_msg_param是按位设置消息参数(具体见文档<服务器消息交互设计>)
 *     0：消息类型，服务器消息或用户消息
 *     1：m_fd类型，socket描述符或玩家的虚拟描述符
 *
 */

#ifndef CNETMESSAGE_H
#define CNETMESSAGE_H

#include "CPacket.h"

enum ENetMessageParam
{
    ENMP_USER   = 0x1,   //用户消息
    ENMP_SOCKE  = 0x2    //socket描述符
};

class CNetMessage : public CPacket
{
public:
    CNetMessage();

    CPacket *get_packet();
    int32 get_file_description();
    int32 set_file_description(int32 fd);

    uint8 get_msg_param();
    uint8 set_msg_param(uint8 param);
    uint8 unset_msg_param(uint8 param);

private:
    uint8 m_msg_param;
    int32 m_fd;
};

#endif // CNETMESSAGE_H
