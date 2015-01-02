#ifndef CLOGSTREAM_H
#define CLOGSTREAM_H

#include <iostream>
#include <fstream>

/**
 * @brief The CLogStream class
 * 日志输出流对象，针对错误、警告
 * 在debug下会同时打印到文件、屏幕
 */
class CLogStream : public std::ostream
{
public:
    /*  C++ 模板不支持分离实现 */
    template<typename T>
    CLogStream &operator << ( T t )
    {

#ifdef DEBUG
        std::cerr << t;
#endif

        m_file << t;
        m_file.flush();

        return *this;
    }

    CLogStream &operator << ( std::ostream& (*pf)(std::ostream&) );

    explicit CLogStream( const char *file );
    ~CLogStream();
private:
    std::ofstream m_file;
};

#endif // CLOGSTREAM_H
