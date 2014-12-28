/*
 * 网络数据包存储类
 * 模板类，不支持分离编译。不允许使用export
 */

#ifndef CPACKET_H
#define CPACKET_H

#include "gstypes.h"

#define PACKET_LENGTH    32768   //长度 32*1024

template <typename T>
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
    T *m_p_length;
};


template <typename T>
CPacket<T>::CPacket()
{
    m_p_length = new(m_buff) T;
    zero();
}

/**
 * @brief CPacket::zero
 * 重置缓冲区指针位置
 */
template <typename T>
void CPacket<T>::zero()
{
    *m_p_length = sizeof(*m_p_length);/* 自己占了包头的长度 */
}

/**
 * @brief CPacket::truncate
 * 重围缓冲区并初始化缓冲区内存
 */
template <typename T>
void CPacket<T>::truncate()
{
    zero();
    memset( m_buff + *m_p_length,0,PACKET_LENGTH ); /* 不要将包头置0 */
}

/**
 * @brief CPacket::get_length
 * @return 缓冲区数据长度(包括包头)
 */
template <typename T>
uint32 CPacket<T>::get_length()
{
    return *m_p_length;
}

/**
 * @brief CPacket::get_packet_length
 * @return 缓冲区数据长度(不包括包头)
 * 获取当前已缓冲的数据长度
 */
template <typename T>
uint32 CPacket<T>::get_packet_length()
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
template <typename T>
bool CPacket<T>::fill_buff( void *p, uint32 length )
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
template <typename T>
char *CPacket<T>::get_buff_ptr()
{
    assert( null != m_buff );

    return m_buff;
}

#endif // CPACKET_H
