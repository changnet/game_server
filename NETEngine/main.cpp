#include <iostream>
#include <unistd.h>
#include <ev++.h>
#include "gslog.h"
#include "CNetBackend.h"
#include "CServerSetting.h"

using namespace std;

int main(int32 argc,char **argv)
{

    CNetBackend worker;

    worker.start_work();
    worker.end_work();

    return 0;
}

