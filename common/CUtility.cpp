#include <cstdio>      /* for snprintf  */
#include <ev++.h>
#include <sys/types.h>
#include <unistd.h>

#include "CUtility.h"

CUtility *CUtility::m_p_utility = null;

CUtility::CUtility()
{
    m_timestamp  = 0;

    pst.tm_sec  = 0;         /* seconds */
    pst.tm_min  = 0;         /* minutes */
    pst.tm_hour = 0;        /* hours */
    pst.tm_mday = 0;        /* day of the month */
    pst.tm_mon  = 0;         /* month */
    pst.tm_year = 0;        /* year */
    pst.tm_wday = 0;        /* day of the week */
    pst.tm_yday = 0;        /* day in the year */
    pst.tm_isdst = 0;       /* daylight saving time */


    sztime[0] = '\0';
}
CUtility::~CUtility()
{
}

CUtility &CUtility::instance()
{
    if ( null == m_p_utility )
        m_p_utility = new CUtility();

    return *m_p_utility;
}

void CUtility::unstance()
{
    if ( m_p_utility )
        delete m_p_utility;

    m_p_utility = null;
}

const char *CUtility::str_time()
{
    return sztime;
}

/**
 * @brief CUtility::update_time
 *
 * localtime:(非线程安全)
 * his structure is statically allocated and shared by the functions gmtime and localtime.
 * Each time either one of these functions is called the content of this structure is overwritten.
 */
void CUtility::update_time()
{
    m_timestamp = static_cast<time_t>(ev_now(EV_DEFAULT));

    localtime_r( &m_timestamp,&pst );

    snprintf( sztime,SZTIME_LENGTH,"%04d-%02d-%02d %02d:%02d:%02d",
             1900+pst.tm_year,
             1+pst.tm_mon,
             pst.tm_mday,
             pst.tm_hour,
             pst.tm_min,
             pst.tm_sec
             );
}
