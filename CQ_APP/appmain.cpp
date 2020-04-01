#include "fun.hpp"
#include "gui.h"

#include "appinfo.h"


using namespace std;
using namespace CQ;

MUST_AppInfo_RETURN(APP_ID)

long long itime = 0;
map<int64_t, Private> insQQ;


void init()
try
{
	conf.file2json();
	conf.json2all();

	string sendStr = "群关键词监控已开启";
	for (auto one_root : conf.admin)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
catch (exception & e)
{
	string sendStr = "出现异常错误:";
	sendStr += e.what();

	logger.Info(sendStr.c_str());
	for (auto one_root : conf.admin)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}


//私聊消息
EVE_PrivateMsg(SendMsg) {

	insQQ[fromAccount].put_fromQQ(fromAccount);
	insQQ[fromAccount].funthing(msg);

	return 0;
}


//群消息
EVE_GroupMsg(GroupMsg)
{
	OperateMsg a(fromAccount, fromAnonymous, fromGroup, msgId, msg);

	a.MsgFun();

	return 0;
}


//菜单
EVE_Menu(menu)
//name:设置
{
	try
	{
		auto readConf([]()
		{
			//修改配置后重新读取配置
			conf.all2json();
			conf.json2file();

			/*for (long long root : g_root)
			{
				CQ::sendPrivateMsg(root, "《群关键词监控》配置已生效");
			}*/
		});



		Gui gui(readConf);
		gui.openMain();

	}
	catch (exception & e)
	{
		MessageBoxA(NULL, e.what(), "界面异常崩溃", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(NULL, "未知原因", "界面异常崩溃", MB_OK);
	}


	return 0;
}











EVE_Startup_EX(Startup)
//name:酷Q启动
//priority:30000
/*
以上2行为注释,用于json自动生成
name为事件名称,一般标识用途,不需要表明事件类型
priority为事件优先级(参见 cq.im/deveventpriority)
开头必须为<//name:>或<//priority:>,不得有空格等
如果没有name,默认为事件名
如果没有priority,默认为30000
不要包含{字符,否则视为结束
*/

{
	logger.Info("Startup");
	//本函数在执行周期最多只会执行一次
	//可以在这里加载全局资源
}


EVE_Enable_EX(Enable)
//name:禁用将无法加载数据
//priority:30000
{
	//获取应用数据目录
	appDir = CQ::getAppDirectory();

	//初始化数据
	init();

	logger.Info("群监控被启用");
	//应用有可能多次启用
	//可以在这里加载运行周期内才需要的资源
}


EVE_Disable_EX(Disable)
//name:禁用将不能保存数据
//priority:30000
{
	logger.Info("群监控被停用");
	//应用有可能多次停止
	//停止后dll还会继续运行
	//但是收不到任何事件也不能调用任何API
}