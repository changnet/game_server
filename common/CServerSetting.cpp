#include "CServerSetting.h"
#include "gslog.h"

CServerSetting *CServerSetting::m_p_server_setting = null;

CServerSetting::CServerSetting()
{
    m_net_port = 0;
    m_server_id = "";

    m_read = read_setting();
}

CServerSetting::~CServerSetting()
{
}

CServerSetting &CServerSetting::instance()
{
    if ( null == m_p_server_setting )
        m_p_server_setting = new CServerSetting();

    return *m_p_server_setting;
}

void CServerSetting::uninstance()
{
    if ( m_p_server_setting )
        delete m_p_server_setting;

    m_p_server_setting = null;
}

bool CServerSetting::read_setting()
{
    boost::property_tree::ptree pt_config;
    try
    {
        read_xml( CONFIGFILE,pt_config );

        m_net_port  = pt_config.get( "setting.NETEngine.port",0 );
        m_server_id = pt_config.get<string>( "setting.serverid" );
        m_data_server_name = pt_config.get<string>( "setting.DataServer.name" );
        m_data_server_path = pt_config.get<string>( "setting.DataServer.path" );
        m_log_engine_name  = pt_config.get<string>( "setting.LOGEngine.name" );
        m_log_engine_path  = pt_config.get<string>( "setting.LOGEngine.path" );
        m_net_engine_name  = pt_config.get<string>( "setting.NETEngine.name" );
        m_net_engine_path  = pt_config.get<string>( "setting.NETEngine.path" );
    }
    catch ( boost::property_tree::ptree_bad_path &e )
    {
        GERROR() << "server.xml path error:" << e.what() << "\n";
        return false;
    }
    catch ( boost::property_tree::ptree_bad_data &e )
    {
        GERROR() << "server.xml data format error:" << e.what() << "\n";
        return false;
    }
    catch ( boost::property_tree::ptree_error &e )
    {
        GERROR() << "read server.xml error:" << e.what() << "\n";
        return false;
    }

    return true;
}

/*
 * 写入默认配置格式
 */
void CServerSetting::save_default_setting()
{
    using boost::property_tree::ptree;

    ptree pt;

    pt.put( "setting.name","untitle" );    //游戏名称
    pt.put( "setting.serverid","game_xzc" );    //服务器标识，用于区分多个服务器

    ptree pt_data_child;
    pt_data_child.put( "name","DataServer" );
    pt_data_child.put( "path","./build/DataServer" );
    pt.add_child( "setting.DataServer",pt_data_child );

    ptree pt_net_child;
    pt_net_child.put( "port",9999 );
    pt_net_child.put( "name","NETEngine" );
    pt_net_child.put( "path","./build/NETEngine" );
    pt.add_child( "setting.NETEngine",pt_net_child );

    ptree pt_log_child;
    pt_log_child.put( "name","LOGEngine" );
    pt_log_child.put( "path","./build/LOGEngine" );
    pt.add_child( "setting.LOGEngine",pt_log_child );

    boost::property_tree::xml_writer_settings<string> settings =
            boost::property_tree::xml_writer_make_settings<string> (' ', 4);
    write_xml( DEFAULTCONFIG,pt,std::locale(),settings );
}

int32 CServerSetting::get_listen_port()
{
    if ( !m_read )
        return 0;

    return m_net_port;
}

const char *CServerSetting::get_server_id()
{
    if ( !m_read )
        return null;

    return m_server_id.c_str();
}

const char *CServerSetting::get_data_server_path()
{
    return m_data_server_path.c_str();
}

const char *CServerSetting::get_data_server_name()
{
    return m_data_server_name.c_str();
}

const char *CServerSetting::get_log_engine_path()
{
    return m_log_engine_path.c_str();
}

const char *CServerSetting::get_log_engine_name()
{
    return m_log_engine_name.c_str();
}

const char *CServerSetting::get_net_engine_path()
{
    return m_net_engine_path.c_str();
}

const char *CServerSetting::get_net_engine_name()
{
    return m_net_engine_name.c_str();
}
