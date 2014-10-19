#include "CPacket.h"

CPacket::CPacket()
{
    zero();
}

/**
 * @brief CPacket::zero
 * 重置缓冲区指针位置
 */
void CPacket::zero()
{
    m_buff_length = 0;
    m_current_pos = 0;
}

/**
 * @brief CPacket::truncate
 * 重围缓冲区并初始化缓冲区内存
 */
void CPacket::truncate()
{
    zero();
    memset( buff,0,MSG_LENGTH );
}

/**
 * @brief CPacket::get_buff
 * @return 缓冲区指针
 */
char *CPacket::get_buff()
{
    return buff;
}

/**
 * @brief CPacket::get_length
 * @return 获取缓冲区数据长度
 */
uint32 CPacket::get_length()
{
    return m_buff_length;
}

/**
 * @brief CPacket::add_length
 * @return 新缓冲区长度
 * 添加缓冲区长度
 */
uint32 CPacket::add_length(uint32 length)
{
    return m_buff_length += length;
}

/**
 * @brief CPacket::set_length
 * @param length
 * @return 新缓冲区长度
 *
 * 设置缓冲区已使用长度
 */
uint32 CPacket::set_length(uint32 length)
{
    return m_buff_length = length;
}
