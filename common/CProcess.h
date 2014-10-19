#ifndef CPROCESS_H
#define CPROCESS_H

#include <ctime>
#include <ev++.h>
#include <sys/types.h>
#include <unistd.h>

#define SZTIME_LENGTH    128    //2014-09-14 15:38:00字符串所分配的内存

class CProcess
{
public:
    static const char *get_process_name();
    static const char *set_process_name( const char *name );
    static const char *get_log_process_id();
private:
    CProcess();

private:
    static const char *m_pprocess_name;
};

#endif // CPROCESS_H
