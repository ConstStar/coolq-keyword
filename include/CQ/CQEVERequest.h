#pragma once
#include <string>
#include "CQEVEBasic.h"
namespace CQ
{
	struct EVERequest :public EVE
	{
		int sendTime; // ����ʱ��(ʱ���)
		long long fromQQ; // ��ԴQQ
		const char* msg; // ����
		const char* responseFlag;// ������ʶ(����������)

		EVERequest(int sendTime, long long fromQQ, const char* msg, const char* responseFlag);
		virtual void pass(std::string msg) = 0;//ͨ��������
		virtual	void fail(std::string msg) = 0;//�ܾ�������
	};
}