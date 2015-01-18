#include <iostream>

#include "CDataBackend.h"
#include "main.h"

using namespace std;

int main(int argc,char **argv)
{
    if ( argc != 2 )
    {
        GERROR() << "run " << ENGINE_NAME << " argument error\n";
        return 0;
    }

    ENGINE_NAME = argv[0];

    CDataBackend worker;

    worker.signal_watch();
    worker.start();

    return 0;
}

