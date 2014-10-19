#include "CServerSetting.h"
#include "gslog.h"

bool CServerSetting::m_is_read = false;
boost::property_tree::ptree CServerSetting::pt_config;

CServerSetting::CServerSetting()
{
}

CServerSetting::~CServerSetting()
{
}

void CServerSetting::read_setting()
{
    try
    {
        read_xml( CONFIGFILE,pt_config );
    }
    catch ( boost::property_tree::ptree_bad_path &e )
    {
        GERROR() << "server.xml path error:" << e.what() << "\n";
        return;
    }
    catch ( boost::property_tree::ptree_bad_data &e )
    {
        GERROR() << "server.xml data format error:" << e.what() << "\n";
        return;
    }
    catch ( boost::property_tree::ptree_error &e )
    {
        GERROR() << "read server.xml error:" << e.what() << "\n";
        return;
    }

    m_is_read = true;
}

/*
 * 写入默认配置格式
 */
void CServerSetting::save_default_setting()
{
    using boost::property_tree::ptree;

    ptree pt;

    pt.put( "setting.name","untitle" );    //游戏名称
    pt.put( "setting.serverid","game1" );    //服务器标识，用于区分多个服务器

    ptree pt_child;

    pt_child.put( "port",9999 );
    pt.add_child( "setting.NETEngine",pt_child );

    write_xml( DEFAULTCONFIG,pt );
}

int32 CServerSetting::get_listen_port()
{
    if ( !m_is_read )
        read_setting();

    return pt_config.get( "setting.NETEngine.port",0 );
}


