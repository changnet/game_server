#include "CLogMessage.h"

#include <cstring>

CLogMessage::CLogMessage()
{
    m_log_file_path[0] = '\0'; //初始化空字符串
}

char *CLogMessage::get_log_path()
{
    return m_log_file_path;
}

/**
 * @brief CLogMessage::set_log_path
 * @param path
 *
 * 对于在一个进程间而言，log的路径基本是常量const char *，但如果路径是从脚本传进来
 * 或从共享内存得到，则必须copy一份
 */
void CLogMessage::set_log_path(const char *path)
{
    int32 length = strlen(path);
    if ( 0 >= length )
        return;

    length = length >= MAX_LOG_FILE_NAME_LENGTH ? MAX_LOG_FILE_NAME_LENGTH-1 : length;

    memcpy( m_log_file_path,path,length );
    m_log_file_path[length+1] = '\0';
}
