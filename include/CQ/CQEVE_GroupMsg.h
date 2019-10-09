#pragma once
//#include <exception>
#include "CQEVE.h"//����ɾ������...
#include "CQEVEMsg.h"

#include <vector>
/*
Ⱥ��Ϣ(Type=2)

subType �����ͣ�Ŀǰ�̶�Ϊ1
msgId ��ϢID
fromGroup ��ԴȺ��
fromQQ ��ԴQQ��
fromAnonymous ��Դ������
msg ��Ϣ����
font ����

�����Ϣ����������,isAnonymous() ���� true, ��ʹ�� getFromAnonymousInfo() ��ȡ ��������Ϣ

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_GroupMsg_EX(Name) 																	\
	void Name(CQ::EVEGroupMsg & eve);															\
	EVE_GroupMsg(Name)																		\
	{																							\
		CQ::EVEGroupMsg tep(subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font);	\
		EVETry Name(tep); EVETryEnd(Name,������һ������)										\
		return tep._EVEret;																		\
	}																							\
	void Name(CQ::EVEGroupMsg & eve)

namespace CQ {
	class GroupMemberInfo;

	// Ⱥ������Ϣ
	struct AnonymousInfo
	{
		�������� AID;
		std::string ����;
		//std::vector<unsigned char> Token;

		AnonymousInfo(const char* msg);
		AnonymousInfo() = default;
	};

	//Ⱥ�¼�
	struct EVEGroupMsg :public EVEMsg
	{
	private:
		AnonymousInfo * fromAnonymousInfo;
	public:
		//Ⱥ��
		long long fromGroup;
		//�����õ�����
		const char* fromAnonymousToken;
		EVEGroupMsg(int subType, int msgId, long long fromGroup, long long fromQQ, const char* fromAnonymous, const char* msg, int Font);

		virtual ~EVEGroupMsg();

		bool isAnonymous() const;

		// ͨ�� EVEMsg �̳�
		int sendMsg(const char *) const override;
		int sendMsg(std::string) const override;
		MsgSend sendMsg() const override;

		//��ȡ��������Ϣ
		AnonymousInfo&getFromAnonymousInfo() /*throw(std::exception_ptr)*/;

		//��ȺԱ�Ƴ�
		bool setGroupKick(bool refusedAddAgain = false);
		//��ȺԱ����
		//�Զ��ж��Ƿ�������
		bool setGroupBan(long long banTime = 60);
		//��Ⱥ����Ա
		bool setGroupAdmin(bool isAdmin);
		//��Ⱥ��Աר��ͷ��
		bool setGroupSpecialTitle(std::string ר��ͷ��, long long ����ʱ�� = -1);

		//��ȫȺ����
		bool setGroupWholeBan(bool �������� = true);
		//��Ⱥ��������
		bool setGroupAnonymous(bool ��������);
		//��Ⱥ��Ա��Ƭ
		bool setGroupCard(std::string ����Ƭ_�ǳ�);
		//��Ⱥ�˳�
		bool setGroupLeave(bool �Ƿ��ɢ);
		//ȡȺ��Ա��Ϣ (֧�ֻ���)
		GroupMemberInfo getGroupMemberInfo(bool ��ʹ�û��� = false);
		//ȡȺ��Ա�б�
		std::vector<GroupMemberInfo> getGroupMemberList();
	};
}
