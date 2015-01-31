#ifndef CRUNTIMESTREAM_H
#define CRUNTIMESTREAM_H

#include <iostream>
#include <fstream>

#define MAX_RUNTIME_PATH_LEN   128

/**
 * @brief The CRuntimeStream class
 * 同时写入文件并在屏幕打印的输出流
 * 非线程安全
 */
class CRuntimeStream : public std::ostream
{
public:

    /*  C++ 模板不支持分离实现 */
    template<typename T>
    CRuntimeStream &operator << ( T t )
    {

        std::cerr << t;

        open();
        m_file << t;
        close();

        return *this;
    }

    CRuntimeStream &operator << ( std::ostream& (*pf)(std::ostream&) );

    explicit CRuntimeStream( const char *file );
private:
    void open();
    void close();
private:
    std::ofstream m_file;
    char m_path[MAX_RUNTIME_PATH_LEN];
};

#endif // CRUNTIMESTREAM_H
