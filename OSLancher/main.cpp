/*
 * 游戏进程启动器
 *
 * OSLancher -start    #开启服务器
 * OSLancher -config   #写入默认配置样本
 */

#include <iostream>

#include "gstypes.h"
#include "gslog.h"
#include "CServerSetting.h"


using namespace std;

int main( int argc,char **argv )
{
    if ( 1 >= argc )  //1 is the grogram name
    {
        GERROR() << "usage:\n";
        GERROR() << "    [-start] start running the server.\n";
        GERROR() << "    [-config] write a config sample to file server_default.xml.\n";

        return 0;
    }

    for ( int i = 1;i < argc;i ++ )    //1 is alway the program name
    {
        if ( 0 == strcmp( argv[i],"-start" ) )    //start server
        {
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

