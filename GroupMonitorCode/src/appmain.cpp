#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

#include <Windows.h>

using namespace std;
using namespace CQ;


extern string AppFile;

//请加上static,表示这个logger只有本cpp有效
static Logger logger("群关键词监控");

extern void init();


//界面
typedef void(*Pwinfun)(const char* Path, const char* fun);
HINSTANCE hDLL;
extern Pwinfun win; //暂时先这样写吧


EVE_Enable(Enable)
{
	//获取应用数据目录
	AppFile = CQ::getAppDirectory();

	//导入界面dll
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //加载动态链接库MyDll.dll文件；

	if (hDLL == NULL) {
		MessageBoxA(NULL, "界面导入失败，可能是你没有复制界面dll\n有问题欢迎到QQ群：839067703 求助", "错误", NULL);
		//logger.Info("界面打开失败");
	}
	else
	{
		//指向dll中打开界面函数
		win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	}

	//初始化数据
	init();

	logger.Info("群监控被启用");

	return 0;
}

EVE_Disable(Disable)
{
	FreeLibrary(hDLL);
	logger.Info("群监控被停用");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)
