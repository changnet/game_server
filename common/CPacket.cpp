#include "CPacket.h"

CPacket::CPacket()
{
    m_p_length = new(m_buff) msghead;
    zero();
}

/**
 * @brief CPacket::zero
 * 重置缓冲区指针位置
 */
void CPacket::zero()
{
    *m_p_length = sizeof(*m_p_length);/* 自己占了包头的长度 */
}

/**
 * @brief CPacket::truncate
 * 重围缓冲区并初始化缓冲区内存
 */
void CPacket::truncate()
{
    zero();
    memset( m_buff + *m_p_length,0,PACKET_LENGTH ); /* 不要将包头置0 */
}

/**
 * @brief CPacket::get_length
 * @return 缓冲区数据长度(包括包头)
 */
uint32 CPacket::get_length()
{
    return *m_p_length;
}

/**
 * @brief CPacket::get_packet_length
 * @return 缓冲区数据长度(不包括包头)
 * 获取当前已缓冲的数据长度
 */
uint32 CPacket::get_packet_length()
{
    return *m_p_length - sizeof(*m_p_length);
}

/**
 * @brief CPacket::fill_buff
 * @param p
 * @param length
 * @return
 * 写入缓冲区
 */
bool CPacket::fill_buff( void *p, uint32 length )
{
    assert ( null != p );

    if ( PACKET_LENGTH < *m_p_length + length )  /* buffer overflow */
        return false;

    memcpy( m_buff + *m_p_length,p,length );

    *m_p_length += length;

    return true;
}

/**
 * @brief CPacket::get_buff_ptr
 * @return 整个缓冲区的指针
 * 获取整个缓冲区指针
 */
char *CPacket::get_buff_ptr()
{
    assert( null != m_buff );

    return m_buff;
}
