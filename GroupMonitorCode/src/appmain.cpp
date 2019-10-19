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

typedef void(*Pwinfun)(const char* Path, const char* fun);

HINSTANCE hDLL;
extern Pwinfun win;

EVE_Enable(Enable)
{
	//获取应用数据目录
	AppFile = CQ::getAppDirectory();

	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //加载动态链接库MyDll.dll文件；

	if (hDLL == NULL) {
		MessageBoxA(NULL, "界面打开失败，可能是你没有复制界面dll\n有问题欢迎到QQ群：839067703 求助", "错误", NULL);
		//logger.Info("界面打开失败");
	}
	else
	{
		win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	}
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
