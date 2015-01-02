#ifndef CRUNTIMESTREAM_H
#define CRUNTIMESTREAM_H

#include <iostream>
#include <fstream>

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

        m_file << t;
        m_file.flush();

        return *this;
    }

    CRuntimeStream &operator << ( std::ostream& (*pf)(std::ostream&) );

    explicit CRuntimeStream( const char *file );
    ~CRuntimeStream();
private:
    std::ofstream m_file;
};

#endif // CRUNTIMESTREAM_H
