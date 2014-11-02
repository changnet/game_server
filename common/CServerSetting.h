/*
 * 读取服务器配置server.xml
 *
 * 在boost中，貌似没法直接从ptree中返回const char*类型，只有std::string类型
 * 所以只有将变量读取到对应的成员变量中
 *
 */

#ifndef CSERVERSETTING_H
#define CSERVERSETTING_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/detail/xml_parser_writer_settings.hpp>
#include "gstypes.h"

#define CONFIGFILE    "server.xml"
#define DEFAULTCONFIG "server_default.xml"

class CServerSetting
{
public:
    static CServerSetting &instance();
    static void uninstance();

    bool read_setting();
    static void save_default_setting();
    int32 get_listen_port();
    const char *get_server_id();
    const char *get_data_server_path();
    const char *get_data_server_name();
    const char *get_log_engine_path();
    const char *get_log_engine_name();
    const char *get_net_engine_path();
    const char *get_net_engine_name();

private:
    static CServerSetting *m_p_server_setting;

    bool   m_read;
    int32  m_net_port;
    string m_server_id;

    string m_data_server_path;
    string m_data_server_name;
    string m_log_engine_name;
    string m_log_engine_path;
    string m_net_engine_name;
    string m_net_engine_path;

    CServerSetting();
    ~CServerSetting();
};

#endif // CSERVERSETTING_H
