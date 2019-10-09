#pragma once
#include "CQEVE.h"//����ɾ������...
//#include "CQEVEMsg.h"
//#include "CQTools.h"
#include "CQEVEMsg.h"
#include "CQMsgSend.h"

/*
˽����Ϣ(Type=21)

�˺����������²���
subType		�����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
msgId	��ϢID
fromQQ		��ԴQQ
msg			˽����Ϣ
font		����

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_PrivateMsg_EX(Name)																	\
	void Name(CQ::EVEPrivateMsg & eve);															\
	EVE_PrivateMsg(Name)																		\
	{																							\
		CQ::EVEPrivateMsg tep(subType, msgId, fromQQ, msg, font);							\
		EVETry Name(tep); EVETryEnd(Name,������һ������)										\
		return tep._EVEret;																		\
	}																							\
	void Name(CQ::EVEPrivateMsg & eve)

namespace CQ {
	struct EVEPrivateMsg :public EVEMsg
	{
		EVEPrivateMsg(int subType, int msgId, long long fromQQ, const char* msg, int Font);

		//���Ժ���
		bool fromPrivate() const;

		//��������״̬
		bool fromOnlineStatus() const;

		//����Ⱥ��ʱ
		bool fromGroup() const;

		//������������ʱ
		bool fromDiscuss() const;

		// ͨ�� EVEMsg �̳�
		virtual MsgSend sendMsg() const override;

		virtual int sendMsg(const char *) const override;

		virtual int sendMsg(std::string) const override;
	};
}
