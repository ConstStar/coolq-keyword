#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

#include <Windows.h>

using namespace std;
using namespace CQ;


extern string AppFile;

//�����static,��ʾ���loggerֻ�б�cpp��Ч
static Logger logger("Ⱥ�ؼ��ʼ��");

extern void init();

typedef void(*Pwinfun)(const char* Path, const char* fun);

HINSTANCE hDLL;
extern Pwinfun win;

EVE_Enable(Enable)
{
	//��ȡӦ������Ŀ¼
	AppFile = CQ::getAppDirectory();

	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //���ض�̬���ӿ�MyDll.dll�ļ���

	if (hDLL == NULL) {
		MessageBoxA(NULL, "�����ʧ�ܣ���������û�и��ƽ���dll\n�����⻶ӭ��QQȺ��839067703 ����", "����", NULL);
		//logger.Info("�����ʧ��");
	}
	else
	{
		win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	}
	init();

	logger.Info("Ⱥ��ر�����");

	return 0;
}

EVE_Disable(Disable)
{
	FreeLibrary(hDLL);
	logger.Info("Ⱥ��ر�ͣ��");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)
