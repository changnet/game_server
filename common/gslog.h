/*
 * INFO    :需要记录的信息，如玩家消耗、产出log。允许指定文件名,有缓存机制
 * WARNING :警告信息，如系统玩家数量达到设定值，所占内存接近上限，某个功能耗时过长。检测到非法链接.即时flush
 * ERROR   :出现错误，但系统尚能运行，如DB引擎写入Sql失败，读取配置失败。或检测到某个子功能异常.即时flush
 * FATAL   :出现错误，系统不能正常运行，如分配内存失败、子进程关闭。服务器通常已crash.即时flush
 *
 * INFO底层不应该使用，其缓存机制在于不同进程的共享内存交互及IO流缓存。但底层暂定义为不缓存机制
 *
 */

#ifndef GSLOG_H
#define GSLOG_H

#include <sys/types.h>
#include <unistd.h>      /* for getpid */

#include "CUtility.h"
#include "CLogStream.h"
#include "CRuntimeStream.h"

// first i define INFO WARNING ERROR,buf libev conflict,say "some systems stupidly #define ERROR"
// G mean GAME

static CRuntimeStream runtime_stream( "runtime" );
static CLogStream     warning_stream( "warning" );
static CLogStream     error_stream  ( "error"   );

extern const char *ENGINE_NAME;
#define GINFO(x)      CLogger::instance()->message(x)
#define GRUNTIME()    runtime_stream
#define GWARNING()    (warning_stream << "[" << CUtility::instance()->str_time() \
                            << getpid() << "-" << ENGINE_NAME << "]")
#define GERROR()      (error_stream << "[" << CUtility::instance()->str_time() \
                            << getpid() << "-" << ENGINE_NAME << "]")

#endif // GSLOG_H
