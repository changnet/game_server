/*
 * log消息，与其他消息不同，log消息无需回回复，故无需带fd
 * 只带文件名即可
 *
 */

#ifndef CLOGMESSAGE_H
#define CLOGMESSAGE_H

#define MAX_LOG_FILE_NAME_LENGTH    64    //最大文件名,不包括路径

#include "CPacket.h"

class CLogMessage : public CPacket
{
public:
    CLogMessage();

    char *get_log_path();
    void set_log_path(const char *path);

    char m_log_file_path[MAX_LOG_FILE_NAME_LENGTH];

};

#endif // CLOGMESSAGE_H
