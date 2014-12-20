#include "CLogMessage.h"
#include <cstdio>         /* for snprintf */

#include "gslog.h"

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%hhu",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%hhd",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%hu",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%hd",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%u",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%d",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%llu",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%lld",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%f",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%f",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%s",val);

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
    int32 cx = snprintf(m_buff+*m_p_length,PACKET_LENGTH-*m_p_length,"%s",str.c_str() );

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
 * 使用std::endl来结束log并换行
 */
void CLogMessage::operator << (std::basic_ostream< char, std::char_traits<char> >& (*_Pfn)
                               (std::basic_ostream<char, std::char_traits<char> > &) )
{
    UNUSED(_Pfn);
    *this << "\n";

    /* 协议约定字符串长度包括最后一个'\0' */
    *m_p_length += 1;
    *m_p_log_length += 1;
}

bool CLogMessage::init( const char *path )
{
    strhead *p_path_length = new(m_buff+*m_p_length) strhead;

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
        *m_p_length += cx + sizeof(strhead);

    }

    m_p_log_length = new(m_buff+*m_p_length+*p_path_length) strhead;
    *m_p_log_length = 0;

    return true;
}

