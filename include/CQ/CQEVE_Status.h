#pragma once

#include "CQEVE.h"//����ɾ������...
#include <string>
/*
������

������ eve.data eve.dataf eve.color �����������ֲ�������

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
*/

#define EVE_Status_EX(Name)					\
	void Name(CQ::EVEStatus & eve);\
	EVE_Status(Name)\
	{\
		CQ::EVEStatus tep;\
		static std::string ret;\
		EVETry Name(tep); EVETryEnd(Name,������һ������)										\
		ret = CQ::statusEVEreturn(tep);\
		return ret.c_str();\
	}\
	void Name(CQ::EVEStatus & eve)

namespace CQ {
	struct EVEStatus
	{
		std::string
			//����
			data,
			//���ݵ�λ
			dataf;
		int
			// 1 : ��
			// 2 : ��
			// 3 : ��
			// 4 : ���
			// 5 : ��
			// 6 : ��
			color;
		// 1 : ��
		void color_green();
		// 2 : ��
		void color_orange();
		// 3 : ��
		void color_red();
		// 4 : ���
		void color_crimson();
		// 5 : ��
		void color_black();
		// 6 : ��
		void color_gray();
	};
	std::string statusEVEreturn(EVEStatus & eve);
}
