#ifndef CDATABACKEND_H
#define CDATABACKEND_H

#include "CLogWorker.h"

class CDataBackend
{
public:
    CDataBackend();

    void start();

private:
    CLogWorker m_log_worker;
};

#endif // CDATABACKEND_H
