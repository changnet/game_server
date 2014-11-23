/*
 * 公共类，提供一些常用的接口，如时间、随机数
 *
 */

#ifndef CUTILITY_H
#define CUTILITY_H

#include <ctime>
#include "gstypes.h"

#define SZTIME_LENGTH    32    //2014-09-14 15:38:00字符串所分配的内存

class CUtility
{
public:
    static CUtility *instance();
    static void unstance();

    const char *str_time();
    time_t time();
    void update_time();

private:
    CUtility();
    ~CUtility();

private:
    time_t m_timestamp;
    struct tm pst;

    char sztime[SZTIME_LENGTH];

    static CUtility *m_p_utility;
};

#endif // CUTILITY_H
