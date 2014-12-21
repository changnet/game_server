#ifndef CBACKEND_H
#define CBACKEND_H

#include "CUtility.h"

/**
 * @brief The CBackend class
 * 后台工作基类，为各个进程后台进程提供共同的功能
 * 包括初始化时间
 * 退出时销毁日志、工具类等对象内存...
 */
class CBackend
{
public:
    explicit CBackend();
    ~CBackend();
    
    void backend();
protected:
    CUtility *m_p_utility;
};

#endif // CBACKEND_H
