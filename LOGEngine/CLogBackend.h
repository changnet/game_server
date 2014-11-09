#ifndef CLOGBACKEND_H
#define CLOGBACKEND_H

#include "gstypes.h"
#include "CSeamphoreLock.h"
#include "CLogWorker.h"

class CLogBackend
{
public:
    CLogBackend();

    void set_self_name(const char *name);
    const char *get_self_name();

    void set_server_id(const char *id);
    const char *get_server_id();

    void set_center_pid(int32 pid);
    int  get_center_pid();

    void start_work();
    void end_work();

private:
    const char *m_self_name;
    const char *m_server_id;
    int32 m_center_pid;

    CLogWorker m_log_worker;
};

#endif // CLOGBACKEND_H
