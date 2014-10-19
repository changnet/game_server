#include "CProcess.h"

#include <cstdio>

const char *CProcess::m_pprocess_name = "";
CProcess::CProcess()
{
}

const char *CProcess::get_process_name()
{
    return m_pprocess_name;
}

const char *CProcess::set_process_name(const char *name)
{
    return m_pprocess_name = name;
}

/* 在log最前面添加进程标识[pid-name 2014-09-14 15:32:00]
 * 用ev_now取代time(0),保证在一次loop里所有时间都是一样的
 */
const char *CProcess::get_log_process_id()
{
    static char sztime[SZTIME_LENGTH] = {'\0'};  //has to be static

    time_t tt = static_cast<time_t>(ev_now(EV_DEFAULT));

    struct tm *pst = localtime( &tt );

    sprintf( sztime,"[%d-%s %04d-%02d-%02d %02d:%02d:%02d]",
             getpid(),        //PID_MAX=0x8000（可改），因此进程号的最大值为0x7fff，即32767
             m_pprocess_name,
             1900+pst->tm_year,
             1+pst->tm_mon,
             pst->tm_mday,
             pst->tm_hour,
             pst->tm_min,
             pst->tm_sec
             );

    return sztime;
}
