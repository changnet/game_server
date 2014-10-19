#include <iostream>
#include <unistd.h>
#include <ev++.h>
#include "gslog.h"
#include "CNetBackend.h"
#include "CServerSetting.h"

using namespace std;

int main(int32 argc,char **argv)
{
    CProcess::set_process_name( argv[0] );

    CNetBackend worker;

    worker.start_work();
    worker.end_work();

    CErrorLog::uninstance();
    CSingletonLog::uninstance();

    return 0;
}

