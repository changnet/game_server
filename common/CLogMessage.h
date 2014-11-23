/*
 * log消息，与其他消息不同，log消息无需回回复，故无需带fd
 * 只带文件名即可
 *
 */

#ifndef CLOGMESSAGE_H
#define CLOGMESSAGE_H

#include "gstypes.h"
#include "CPacket.h"

#define MAX_PATH    32

class CLogMessage : public CPacket
{
public:
    CLogMessage();

    void zero();
    void set_path(const char *path);
    const char *get_path();
    uint32 get_path_length();

    CLogMessage &operator << ( uint8 val );
    CLogMessage &operator << ( int8 val );
    CLogMessage &operator << ( uint16 val );
    CLogMessage &operator << ( int16 val );
    CLogMessage &operator << ( uint32 val );
    CLogMessage &operator << ( int32 val );
    CLogMessage &operator << ( uint64 val );
    CLogMessage &operator << ( int64 val );
    CLogMessage &operator << ( float val );
    CLogMessage &operator << ( double val );
    CLogMessage &operator << ( const char *val );
    CLogMessage &operator << ( string &str );

private:
    char m_path[MAX_PATH];
    uint32 m_path_length;

};

#endif // CLOGMESSAGE_H
