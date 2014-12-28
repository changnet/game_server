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

class CNetMessage : public CPacket<nethead>
{
public:
    CNetMessage();

    uint8 get_msg_param();
    uint8 set_msg_param(uint8 param);
    uint8 unset_msg_param(uint8 param);

    char *get_free_net_msg_ptr();
    char *get_net_msg_ptr(uint32 length);
    uint32 get_free_net_msg_length();

    bool add_buff_length( uint32 length );
    uint32 get_net_msg_length();

private:
    uint8 m_msg_param;
};

#endif // CNETMESSAGE_H
