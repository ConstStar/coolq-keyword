#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

using namespace std;
using namespace CQ;


extern string AppFile;

//�����static,��ʾ���loggerֻ�б�cpp��Ч
static Logger logger("Ⱥ�ؼ��ʼ��");

extern void init();

EVE_Enable(Enable)
{
	//��ȡӦ������Ŀ¼
	AppFile = CQ::getAppDirectory();

	init();

	logger.Info("Ⱥ��ر�����");

	return 0;
}

EVE_Disable(Disable)
{

	logger.Info("Ⱥ��ر�ͣ��");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)