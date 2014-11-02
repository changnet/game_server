#include <iostream>
#include "gslog.h"
#include "gstypes.h"
#include "CLogBackend.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 *
 * LOGEngine will start like LOGEngine 16045 server1
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

    CLogBackend worker;

    worker.set_self_name( argv[0] );
    worker.set_center_pid( atoi(argv[1]) );
    worker.set_server_id( argv[2] );

    worker.start_work();
    worker.end_work();

    std::cout << "log engine run..." << std::endl;

    return 0;
}

