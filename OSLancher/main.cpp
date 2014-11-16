/*
 * 游戏进程启动器
 *
 * OSLancher -start    #开启服务器
 * OSLancher -config   #写入默认配置样本
 */

#include <iostream>
#include <unistd.h>        /* for fork */
#include <cstdio>          /* for snprintf */

#include "gstypes.h"
#include "gslog.h"
#include "CServerSetting.h"
#include "main.h"

using namespace std;

#define MAX_PATH_LENGTH    128

void start();

int main( int argc,char **argv )
{
    if ( 1 >= argc )  //1 is the grogram name
    {
        std::cout << "wrong param,please try agin!\n";
        std::cout << "usage:\n";
        std::cout << "    [-start] start running the server.\n";
        std::cout << "    [-config] write a config sample to file server_default.xml.\n";

        return 0;
    }

    for ( int i = 1;i < argc;i ++ )    //1 is alway the program name
    {
        if ( 0 == strcmp( argv[i],"-start" ) )    //start server
        {
            start();
        }
        else if ( 0 == strcmp( argv[i],"-config" ) )    //write default config
        {
            CServerSetting::save_default_setting();

            cout << "the config sample had beed write to server.xml" << endl;
        }
        else
        {
            GERROR() << "invalid argument:" << argv[i] << "\n";
            return 0;
        }
    }

    return 0;
}

void start()
{
    pid_t pid =fork();
    if ( pid<0 )
    {
        GERROR() << "start server fork error" << std::endl;
        return;
    }
    else if ( pid > 0 )  //父进程退出，子进程脱离终端shell,ctrl+c或关闭终端shell不会终止程序
        exit( 0 );

    //在Qt creator开启的shell中仍会终止掉程序，在用户终端则不会
    setsid();  //创建新会话，当前会话结束（用户登出）程序也不会终止

    /*
     * 当一个会话产生（如用户登录），会有一个默认的mask权限值，该值决定了在此会话中对文件操作的权限
     * umask(0)就是设置允许当前进程创建文件或者目录最大可操作的权限，比如这里设置为0，它的意思就是0
     * 取反再创建文件时权限相与，也就是：(~0) & mode 等于八进制的值0777 & mode了
     * 如果不需要，可以不设置，用继承的即可
     *
     */
    //umask(0);  // 设置权限掩码
    //chdir("/");  // 设置工作目录,没必要

    /*
     * 关闭文件描述符，这里还是需要用到std::cout std::cerr
     * 可以关闭std::cin
     * 没什么必要
    for (int fd=0;fd < getdtablesize();fd++ )
    {
        close(fd);
    }
    */

    /*
     * 本进程启动数据服进程作为主服务器，其他进程由数据服启动
     * 本进程的作用到此结束
     *
     */

    char name[MAX_PATH_LENGTH] = {'\0'};
    char sid[MAX_PATH_LENGTH]  = {'\0'};

    snprintf( name,MAX_PATH_LENGTH,"%s",CServerSetting::instance().get_data_server_name() );
    snprintf( sid,MAX_PATH_LENGTH,"%s",CServerSetting::instance().get_server_id() );

    //调用execv会把原进程的所有资源（包括内存）都释放掉
    char* const argv[] = { name,sid,null };
    if ( execv( CServerSetting::instance().get_data_server_path(),argv ) < 0 )
    {
        GERROR() << "execv data server fail:" << strerror( errno ) << std::endl;
        return;
    }

    std::cout << "start server fail:" << strerror( errno ) << std::endl;

    return;
}
