#pragma once
#include "CQEVE.h"//����ɾ������...
#include "CQEVEBasic.h"

/*
�����¼�-���������(Type=201)

subtype �����ͣ�Ŀǰ�̶�Ϊ1
msgId ��ϢID
fromQQ ��ԴQQ

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_Friend_Add_EX(Name) \
	void Name(CQ::EVERequestAddFriend & eve);\
	EVE_Friend_Add(Name)\
	{\
		CQ::EVEFriendAdd tep(subType, msgId, fromGroup, fromQQ, msg, responseFlag);\
		EVETry Name(tep); EVETryEnd(Name,������һ������)										\
		return tep._EVEret;\
	}\
	void Name(CQ::EVEFriendAdd & eve)\

namespace CQ
{
	struct EVEFriendAdd :public EVE {
	};
}
