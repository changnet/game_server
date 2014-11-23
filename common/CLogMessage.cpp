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
    m_path_length = 0;
}

CLogMessage &CLogMessage::operator << ( uint8 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%hhu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint8 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( int8 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%hhd",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int8 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint16 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%hu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint16 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( int16 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%hd",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int16 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint32 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%u",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint32 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( int32 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%d",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int32 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( uint64 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%llu",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf uint64 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( int64 val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%lld",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf int64 error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( float val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%f",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf float error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( double val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%f",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf double error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( const char *val )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%s",val);

    if ( cx < 0 )
    {
        GERROR() << "snprintf char* error\n";
    }
    else
        length += cx;

    return *this;
}

CLogMessage &CLogMessage::operator << ( string &str )
{
    int32 cx = snprintf(buff+length,MSG_LENGTH-length,"%s",str.c_str() );

    if ( cx < 0 )
    {
        GERROR() << "snprintf string error\n";
    }
    else
        length += cx;

    return *this;
}

/**
 * @brief CLogMessage::set_path
 * @param path
 *        The  functions  snprintf()  and  vsnprintf()  write  at most size bytes
 *     (including the terminating null byte ('\0')) to str
 */
void CLogMessage::set_path(const char *path)
{
    int32 cx = snprintf(m_path,MAX_PATH,"%s",path);
    if ( cx < 0 )
    {
        GERROR() << "error while set log message path\n";
        m_path_length = 0;
        return;
    }

    m_path_length = cx;
}

const char *CLogMessage::get_path()
{
    return m_path;
}

uint32 CLogMessage::get_path_length()
{
    return m_path_length;
}
