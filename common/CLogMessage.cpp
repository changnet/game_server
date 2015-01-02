#include "CLogMessage.h"
#include <cstdio>         /* for snprintf */

#include "gslog.h"

#define LOG_POS    (*m_p_length-1)  /* 需要把最后一个\0覆盖掉 */

CLogMessage::CLogMessage()
{
    zero();
}

void CLogMessage::zero()
{
    CPacket::zero();
    m_p_log_length = null;
}

CLogMessage &CLogMessage::operator << ( uint8 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%hhu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint8 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( int8 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%hhd",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int8 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint16 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%hu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint16 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( int16 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%hd",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int16 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint32 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%u",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint32 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( int32 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%d",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int32 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint64 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%llu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint64 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( int64 val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%lld",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int64 error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( float val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%f",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf float error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( double val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%f",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf double error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( const char *val )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%s",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf char* error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

CLogMessage &CLogMessage::operator << ( string &str )
{
    int32 cx = snprintf(m_buff+LOG_POS,PACKET_LENGTH-LOG_POS,"%s",str.c_str() );

    if ( cx < 0 )
    {
        GERROR() << "snprintf string error\n";
    }
    else
    {
        *m_p_length += cx;
        *m_p_log_length += cx;
    }

    return *this;
}

/**
 * @brief CLogMessage::operator <<
 * 使用std::endl来结束log并换行,故不再返回 *this
 */
void CLogMessage::operator << (std::basic_ostream< char, std::char_traits<char> >& (*_Pfn)
                               (std::basic_ostream<char, std::char_traits<char> > &) )
{
    UNUSED(_Pfn);
    *this << "\n";
}

bool CLogMessage::init( const char *path )
{
    strhead *p_path_length = new(m_buff+*m_p_length) strhead;
    *m_p_length += sizeof(strhead);

    /* snprintf
     * Upon successful return, these functions return the number of characters
       printed (excluding the null byte used to end output to strings).

       The  functions  snprintf()  and vsnprintf() do not write more than size
       bytes (including the terminating null byte ('\0')).
    */
    int32 cx = snprintf(m_buff+*m_p_length,MAX_LOG_PATH,"%s",path);

    if ( cx < 0 )
    {
        GERROR() << "snprintf char* error\n";
        return false;
    }
    else
    {
        //如果协议约定需要写入字符串最后一个\0,则需要cx++
        cx++;
        *p_path_length = cx;
        *m_p_length += cx;

    }

    m_p_log_length = new(m_buff+*m_p_length) strhead;
    *m_p_length += sizeof(strhead);

    uint32 size = sizeof( '\0' );
    m_buff[*m_p_length] = '\0';  //把日志初始化为空串

    *m_p_log_length = size;
    *m_p_length += size;

    return true;
}

