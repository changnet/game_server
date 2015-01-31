#include <iostream>
#include <cstring>
#include <cerrno>
#include "gslog.h"
#include "fatal.h"
#include "CLogger.h"

using namespace std;

const char *ENGINE_NAME  = "common";
const char *RUNTIME_FILE = "common_runtime";
const char *WARNING_FILE = "common_warning";
const char *ERROR_FILE   = "common_error";

int main()
{
    cout << "version:" << __cplusplus<<endl;
    GINFO( "user.txt" ) << "I coming soon";
    GWARNING() << "running out of time" << endl;
    GERROR() << "error comming" << endl;
    GRUNTIME() << "shutdown now ..." << endl << "maybe later ..." << endl;

    uint8 val1 = 2;
    int8 val2 = 3;
    uint16 val3 = 65500;
    int16 val4 = 9089;
    uint32 val5 = 19999987;
    int32 val6 = 67876;
    uint64 val7 = 8888789;
    int64 val8 = 100111001;
    const char *p = "haha";
    string str = "what is it";

    int64 t = 0;
    cout << sizeof( t ) << endl;

    GINFO("money.txt") << val1 << val2 << val3 << val4 << val5 << val6 << val7 << val8 << p << str;
    fatal( "nothing" );

    return 0;
}

