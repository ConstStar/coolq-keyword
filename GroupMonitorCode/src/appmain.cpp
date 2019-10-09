#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

using namespace std;
using namespace CQ;


extern string AppFile;

//请加上static,表示这个logger只有本cpp有效
static Logger logger("群关键词监控");

extern void init();

EVE_Enable(Enable)
{
	//获取应用数据目录
	AppFile = CQ::getAppDirectory();

	init();

	logger.Info("群监控被启用");

	return 0;
}

EVE_Disable(Disable)
{

	logger.Info("群监控被停用");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)