#include "CBackend.h"

#include "CLogFile.h"
#include "CLogger.h"

/**
 * @brief CBackend::CBackend
 * 初始化后台工作进程
 */
CBackend::CBackend()
{
    m_p_utility = CUtility::instance();

    m_p_utility->update_time();  //初始化时间戳
}

/**
 * @brief CBackend::~CBackend
 * 后台工作进程清理
 */
CBackend::~CBackend()
{
    CLogFile::unistance();
    CLogger::uninstance();
    CUtility::unstance();

    m_p_utility = null;
}

/**
 * @brief CBackend::backend
 * 执行后台工作中的共同事件
 */
void CBackend::backend()
{
    m_p_utility->update_time();
}
