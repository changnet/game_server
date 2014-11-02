#include <iostream>
#include <cstring>
#include <cerrno>
#include "gslog.h"

#include "CLogger.h"

using namespace std;

int main()
{
    cout << "version:" << __cplusplus<<endl;

    GINFO( "user.txt" ) << "I coming soon";
    GWARNING() << "running out of time" << endl;
    GERROR() << "error comming" << endl;
    GFATAL() << "shutdown now ..." << endl;

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

    CLogger::instance().write_log_to_shm();

    return 0;
}

