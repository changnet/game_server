#include <iostream>
#include <cstring>
#include <cerrno>
#include "gslog.h"
#include "CSeamphoreLock.h"
#include "CSharedMemory.h"

using namespace std;

int main(int32 argc,char ** argv)
{
    cout << "version:" << __cplusplus<<endl;

    CProcess::set_process_name( "COMMON" );

    CErrorLog::uninstance();
    CSingletonLog::uninstance();

    sleep( 2 );

    return 0;
}

