#include "CBackend.h"
#include <cstdlib>    /* fro exit */
#include "CLogger.h"
#include "signal++.h"
#include "gslog.h"

/**
 * @brief CBackend::CBackend
 * 初始化后台工作进程
 */
CBackend::CBackend()
{
    loop = EV_DEFAULT;

    m_utility = CUtility::instance();

    m_utility->update_time();  //初始化时间戳
}

/**
 * @brief CBackend::~CBackend
 * 后台工作进程清理
 */
CBackend::~CBackend()
{
    CLogger::uninstance();
    CUtility::unstance();

    m_utility = null;
}

/**
 * @brief CBackend::backend
 * 执行后台工作中的共同事件
 */
void CBackend::backend()
{
    m_utility->update_time();
}

/**
 * @brief CBackend::abort_backend
 * 紧急中止，发生严重错误时调用，不存数据，不释放资源
 */
void CBackend::abort_backend()
{
    on_exit();

    CSIG_DEFAULT->reset( SIGABRT ); /* abort函数就是发送SIGABRT，故需要先解除 */
    abort();    /* call abort to generate a core-dump */
}

/**
 * @brief CBackend::signal_watch
 * @return
 * 捕捉系统信号并处理
 */
bool CBackend::signal_watch()
{
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGHUP,SA_RESTART );   /* 终端挂起或者控制进程终止  */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGINT,SA_RESTART );   /* 键盘中断(如break键被按下) */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGQUIT,SA_RESTART );  /* 键盘的退出键被按下 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGILL,SA_RESETHAND ); /* 非法指令 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGABRT,SA_RESETHAND ); /* 由abort(3)发出的退出指令 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGFPE,SA_RESETHAND ); /* 浮点异常 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGSEGV,SA_RESETHAND ); /* 无效的内存读写 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGPIPE,SA_RESETHAND ); /* 管道破裂: 写一个没有读端口的管道 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGTERM,SA_RESETHAND ); /* 终止,killall 或 kill不加参数 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGUSR1,SA_RESTART ); /* 用户自定义信号1 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGUSR2,SA_RESTART ); /* 用户自定义信号2 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGCHLD,SA_RESETHAND ); /* 子进程结束信号 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGCONT,SA_RESTART ); /* 进程继续(曾被停止的进程) */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGTSTP,SA_RESTART ); /* 控制终端(tty)上按下停止键 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGTTIN,SA_RESETHAND ); /* 后台进程企图从控制终端读 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGTTOU,SA_RESETHAND ); /* 后台进程企图从控制终端写 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGBUS,SA_RESETHAND ); /* 总线错误(错误的内存访问) */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGXCPU,SA_RESETHAND ); /* 超出设定的CPU时间限制 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGXFSZ,SA_RESETHAND ); /* 超出设定的文件大小限制 */
    //CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGIOT,SA_RESETHAND ); /* IO捕获指令，与SIGABRT同义 */
    CSIG_DEFAULT->set< CBackend,&CBackend::signal_handler > ( this,SIGPWR,SA_RESETHAND ); /* 电源故障(System V) */

    /* 下面的信号不能捕捉,部分信号如SIGKILL不能捕捉
     * SIGALRM、SIGVTALRM 代码里可能使用了定时器
     * SIGPOLL、SIGIO  可以使用该信号来实现异步
     * SIGPROF 部分性能测试工具使用该信号
     * SIGTRAP 程序要断点调试
     */

    return true;
}

/**
 * @brief CBackend::signal_handler
 * @param signum
 * 信号处理总入口
 */
void CBackend::signal_handler( int signum )
{
    switch ( signum )
    {
    case SIGHUP :
    case SIGINT :
    case SIGQUIT :
    case SIGCONT :
    case SIGTSTP :
    case SIGUSR1 :
    case SIGUSR2 :
        GRUNTIME() << "catch signal " << signum << " -- " << strsignal( signum ) << std::endl;
        return;
        break;

    case SIGILL :
    case SIGABRT :
    case SIGSEGV :
    case SIGPIPE :
    case SIGCHLD :
    case SIGTTIN :
    case SIGTTOU :
    case SIGXCPU :
    case SIGXFSZ :
    case SIGPWR :
        GERROR() << "catch signal " << signum << " -- " << strsignal( signum ) << std::endl;
        abort_backend();
        break;
/*
 * 如果信号不足以查找问题，请启用sigaction的SA_SIGINFO并分析siginfo_t
 * 如果还不行，使用backtrace函数
 */

    case SIGTERM :         //正常关闭
        GRUNTIME() << "catch signal " << signum << " -- " << strsignal( signum ) << std::endl;
        stop();
        break;
    }
}
