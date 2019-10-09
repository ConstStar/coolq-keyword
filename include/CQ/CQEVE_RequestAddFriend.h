#pragma once
#include <string>
#include "CQEVE.h"//����ɾ������...
#include "CQEVERequest.h"

/*
����-�������(Type=301)

subtype �����ͣ�Ŀǰ�̶�Ϊ1
sendTime ����ʱ��(ʱ���)
fromQQ ��ԴQQ
msg ����
responseFlag ������ʶ(����������)

�ú���������� (responseFlag, #����_ͨ��)

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_Request_AddFriend_EX(Name) \
	void Name(CQ::EVERequestAddFriend & eve);\
	EVE_Request_AddFriend(Name)\
	{\
		CQ::EVERequestAddFriend tep(subType, sendTime, fromGroup, fromQQ, msg, responseFlag);\
		EVETry Name(tep); EVETryEnd(Name,������һ������)										\
		return tep._EVEret;\
	}\
	void Name(CQ::EVERequestAddFriend & eve)\

namespace CQ
{
	struct EVERequestAddFriend :public EVERequest
	{
		//������
		//1:�̶�Ϊ1
		int subType;
		long long fromGroup; // ��ԴȺ��
		EVERequestAddFriend(int subType, int sendTime, long long fromGroup, long long fromQQ, const char* msg, const char* responseFlag);
		void pass(std::string msg = "")override;//ͨ��������
		void fail(std::string msg = "�����ڲ�����ĳЩҪ�󱻾ܾ�!") override;//�ܾ�������
	};
}