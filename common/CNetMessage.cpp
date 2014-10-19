#include "CNetMessage.h"

CNetMessage::CNetMessage()
{
    m_msg_param = 0;
    m_fd        = 0;
}

CPacket *CNetMessage::get_packet()
{
    return static_cast<CPacket *>( this );
}

int32 CNetMessage::get_file_description()
{
    return m_fd;
}

int32 CNetMessage::set_file_description(int32 fd)
{
    return m_fd = fd;
}

uint8 CNetMessage::get_msg_param()
{
    return m_msg_param;
}

uint8 CNetMessage::set_msg_param(uint8 param)
{
    return m_msg_param |= param;
}

uint8 CNetMessage::unset_msg_param(uint8 param)
{
    return m_msg_param &= ~param;
}
