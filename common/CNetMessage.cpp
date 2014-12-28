#include "CNetMessage.h"

CNetMessage::CNetMessage()
{
    m_msg_param = 0;
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

/**
 * @brief CNetMessage::get_free_buff_ptr
 * @return 空闲缓冲区起始指针
 * 获取当前空闲缓冲区位置的指针
 */
char *CNetMessage::get_free_net_msg_ptr()
{
    return m_buff + *m_p_length;
}

/**
 * @brief CNetMessage::get_free_buff_length
 * @return 当前缓冲区可用长度
 */
uint32 CNetMessage::get_free_net_msg_length()
{
    return PACKET_LENGTH - *m_p_length;
}

/**
 * @brief CNetMessage::add_buff_length
 * @param length
 * @return 是否成功
 * 在直接写入缓冲后再增加缓冲区长度记录
 */
bool CNetMessage::add_buff_length(uint32 length)
{
    if ( PACKET_LENGTH < *m_p_length + length )
        return false;

    *m_p_length += length;

    return true;
}

/**
 * @brief CNetMessage::get_msg_length
 * @return 消息的长度
 * 获取当前消息的长度
 * 确认缓冲区中已写入长度才能调用本函数
 */
uint32 CNetMessage::get_net_msg_length()
{
    return *reinterpret_cast<nethead *>(m_buff+sizeof(*m_p_length));
}

/**
 * @brief CNetMessage::get_buff_ptr
 * @param length 不包含缓冲区本身长度
 * @return 指定位置的消息缓冲区指针
 * 获取指定位置的消息缓冲区指针
 */
char *CNetMessage::get_net_msg_ptr(uint32 length)
{
    assert( PACKET_LENGTH >= sizeof( *m_p_length ) + length );

    return m_buff + sizeof( *m_p_length ) + length;
}
