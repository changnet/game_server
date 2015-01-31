#ifndef CLOGSTREAM_H
#define CLOGSTREAM_H

#include <iostream>
#include <fstream>

#define MAX_LOG_PATH_LEN 128

/**
 * @brief The CLogStream class
 * 日志输出流对象，针对错误、警告
 * 在debug下会同时打印到文件、屏幕
 * 为了保证外部删除、修改日志文件也能正常写入
 * 需要每次都打开、关闭文件，需保证只在错误、警告
 * 这些极少情况下使用，否则有严重性能问题。
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

        open();
        m_file << t;
        close();

        return *this;
    }

    CLogStream &operator << ( std::ostream& (*pf)(std::ostream&) );

    explicit CLogStream( const char *file );
private:
    void open();
    void close();
private:
    char m_path[MAX_LOG_PATH_LEN];
    std::ofstream m_file;
};

#endif // CLOGSTREAM_H
