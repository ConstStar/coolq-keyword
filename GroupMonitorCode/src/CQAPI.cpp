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

//�����static,��ʾ���loggerֻ�б�cpp��Ч
//static Logger logger("Ⱥ�ؼ��ʼ��");


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

	string sendStr = "Ⱥ�ؼ��ʼ���ѿ���";
	for (auto one_root : g_root)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
catch (exception &e)
{
	string sendStr = "�����쳣����:";
	sendStr += e.what();

	logger.Info(sendStr.c_str());
	for (auto one_root : g_root)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
