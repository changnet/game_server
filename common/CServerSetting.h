/*
 * 读取服务器配置server.xml
 *
 */

#ifndef CSERVERSETTING_H
#define CSERVERSETTING_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "gstypes.h"

#define CONFIGFILE    "server.xml"
#define DEFAULTCONFIG "server_default.xml"

class CServerSetting
{
public:
    static void read_setting();
    static void save_default_setting();
    static int32 get_listen_port();

private:
    static bool m_is_read;
    static boost::property_tree::ptree pt_config;

    CServerSetting();
    ~CServerSetting();
};

#endif // CSERVERSETTING_H
