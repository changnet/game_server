#include <iostream>
#include "gslog.h"
#include "gstypes.h"
#include "CLogBackend.h"
#include "main.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 *
 * 启动参数
 * LOGEngine serverid
 *
 * 程序名 服务器标识
 */
int32 main(int32 argc,char **argv)
{
    if ( argc != 2 )
    {
        GERROR() << "wrong param,LOGEngine exit\n";
        return 0;
    }

    ENGINE_NAME = argv[0];
    CLogBackend worker;

    worker.start_work();
    worker.end_work();

    std::cout << "log engine run..." << std::endl;

    return 0;
}

