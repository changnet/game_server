/*
 * log消息，与其他消息不同，log消息无需回回复，故无需带fd
 * 只带文件名即可
 *
 */

#ifndef CLOGMESSAGE_H
#define CLOGMESSAGE_H

#include <iostream>

#include "gstypes.h"
#include "CPacket.h"

#define MAX_LOG_PATH    32

class CLogMessage : public CPacket<msghead>
{
public:
    CLogMessage();

    void zero();
    bool init(const char *path);
    void endl();

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
    void operator << ( std::basic_ostream< char, std::char_traits<char> >& (*_Pfn)
                       (std::basic_ostream< char, std::char_traits<char> >&));

private:
    strhead *m_p_log_length;

};

#endif // CLOGMESSAGE_H
