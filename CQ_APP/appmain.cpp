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

	string sendStr = "Ⱥ�ؼ��ʼ���ѿ���";
	for (auto one_root : conf.admin)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}
catch (exception & e)
{
	string sendStr = "�����쳣����:";
	sendStr += e.what();

	logger.Info(sendStr.c_str());
	for (auto one_root : conf.admin)
	{
		CQ::sendPrivateMsg(one_root, sendStr);
	}
}


//˽����Ϣ
EVE_PrivateMsg(SendMsg) {

	insQQ[fromAccount].put_fromQQ(fromAccount);
	insQQ[fromAccount].funthing(msg);

	return 0;
}


//Ⱥ��Ϣ
EVE_GroupMsg(GroupMsg)
{
	OperateMsg a(fromAccount, fromAnonymous, fromGroup, msgId, msg);

	a.MsgFun();

	return 0;
}


//�˵�
EVE_Menu(menu)
//name:����
{
	try
	{
		auto readConf([]()
		{
			//�޸����ú����¶�ȡ����
			conf.all2json();
			conf.json2file();

			/*for (long long root : g_root)
			{
				CQ::sendPrivateMsg(root, "��Ⱥ�ؼ��ʼ�ء���������Ч");
			}*/
		});



		Gui gui(readConf);
		gui.openMain();

	}
	catch (exception & e)
	{
		MessageBoxA(NULL, e.what(), "�����쳣����", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(NULL, "δ֪ԭ��", "�����쳣����", MB_OK);
	}


	return 0;
}











EVE_Startup_EX(Startup)
//name:��Q����
//priority:30000
/*
����2��Ϊע��,����json�Զ�����
nameΪ�¼�����,һ���ʶ��;,����Ҫ�����¼�����
priorityΪ�¼����ȼ�(�μ� cq.im/deveventpriority)
��ͷ����Ϊ<//name:>��<//priority:>,�����пո��
���û��name,Ĭ��Ϊ�¼���
���û��priority,Ĭ��Ϊ30000
��Ҫ����{�ַ�,������Ϊ����
*/

{
	logger.Info("Startup");
	//��������ִ���������ֻ��ִ��һ��
	//�������������ȫ����Դ
}


EVE_Enable_EX(Enable)
//name:���ý��޷���������
//priority:30000
{
	//��ȡӦ������Ŀ¼
	appDir = CQ::getAppDirectory();

	//��ʼ������
	init();

	logger.Info("Ⱥ��ر�����");
	//Ӧ���п��ܶ������
	//����������������������ڲ���Ҫ����Դ
}


EVE_Disable_EX(Disable)
//name:���ý����ܱ�������
//priority:30000
{
	logger.Info("Ⱥ��ر�ͣ��");
	//Ӧ���п��ܶ��ֹͣ
	//ֹͣ��dll�����������
	//�����ղ����κ��¼�Ҳ���ܵ����κ�API
}