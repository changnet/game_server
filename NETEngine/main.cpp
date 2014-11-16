#include <iostream>
#include <unistd.h>
#include <ev++.h>
#include "gslog.h"
#include "CNetBackend.h"
#include "CServerSetting.h"
#include "main.h"

using namespace std;

int main(int32 argc,char **argv)
{
    if ( argc != 2 )
    {
        GERROR() << "run " << ENGINE_NAME << " argument error\n";
        return 0;
    }

    ENGINE_NAME = argv[0];

    CNetBackend worker;

    worker.start_work();
    worker.end_work();

    return 0;
}

