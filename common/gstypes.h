/*
 * 服务器基本数据类型。
 *先将常用数据类型char short int long float 之类的转换为固定长度类型 type_t
 *再将type_t转换为一个简单易书写的类型
 *
 *about NULL,from Bjarne Stroustrup:
 *    In C++, the definition of NULL is 0, so there is only an aesthetic difference. I prefer to avoid macros,
 *so I use 0. Another problem with NULL is that people sometimes mistakenly believe that it is different
 *from 0 and/or not an integer. In pre-standard code, NULL was/is sometimes defined to something unsuitable
 *and therefore had/has to be avoided. That's less common these days.
 *    If you have to name the null pointer, call it nullptr; that's what it's going to be called in C++0x.
 *Then, "nullptr" will be a keyword.
 */

#ifndef __GSTYPES_H__
#define __GSTYPES_H__

#include <cstdio>
#include <string>

#ifdef NULL
#undef NULL		/* in case <stdio.h> has defined it. or stddef.h */
#endif

#if __cplusplus < 201103L    //-std=gnu99
    #include <stdint.h>
    #define NULL    0
#else
    #include <cstdint>    //if support C++ 2011
    #define NULL    nullptr
#endif

//char short int long already typedef,see /usr/include/stdint.h for more

/*
//char
typedef unsigned char        uint8_t;
typedef signed char          int8_t;
typedef char                 int8_t;

//long
typedef unsigned long         uint32_t;
typedef signed long           int32_t;
typedef long                  int32;

//int
typedef unsigned int          uint32_t;
typedef signed int            int32_t;
typedef int                   int32_t;
typedef unsigned short int    uint64_t;
typedef short int             int64_t;
typedef signed short int      int64_t;

//short
typedef unsigned short        uint16_t;
typedef short                 int16_t;
typedef signed short          int16_t;

*/

//base type
typedef uint8_t        uint8;
typedef int8_t          int8;
typedef uint16_t      uint16;
typedef int16_t        int16;
typedef uint32_t      uint32;
typedef int32_t        int32;
typedef uint64_t      uint64;
typedef int64_t        int64;

/*
 * use std::string instead of C strings,you may use char array like char buff[].don.t use c type strings in cstring(string.h)
 * for my test,a std::string like "code\0more",c_str() return code\0,size() return 4.
 * test like this:
 * string strTest = "code\0more";
 * if ( '\0' == strTest.c_str()[ strTest.size() ] )
 *    cout << "yes" <<endl;
 */
typedef std::string    string;

#define null    NULL

//typedef true    true
//typedef false   false
//typedef bool       bool
//typedef float      float
//typedef double     double

#endif    //for __GSTYPES_H__
