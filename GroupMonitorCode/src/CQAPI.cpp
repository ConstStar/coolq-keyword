#include <CQEVE_PrivateMsg.h>
#include <CQcode.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>

#include "fun.hpp"

#include <string>
#include <time.h>
#include <map>

using namespace std;
using namespace CQ;

//请加上static,表示这个logger只有本cpp有效
//static Logger logger("群关键词监控");


long long itime = 0;
map<int64_t, Private> insQQ;

EVE_PrivateMsg(SendMsg) {

	insQQ[fromQQ].put_fromQQ(fromQQ);
	insQQ[fromQQ].funthing(msg);

	return 0;
}

EVE_GroupMsg(GroupMsg)
{
	OperateMsg a(fromQQ, fromAnonymous, fromGroup, msgId, msg);

	a.MsgFun();

	return 0;
}

void init()
try
{
	OperateFile::file2json();
	OperateFile::json2all();

	string sendStr = "群关键词监控已开启";
	for (auto one_root : g_root)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
catch (exception &e)
{
	string sendStr = "出现异常错误:";
	sendStr += e.what();

	logger.Info(sendStr.c_str());
	for (auto one_root : g_root)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
