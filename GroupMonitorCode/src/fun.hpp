#pragma once
#include <CQEVE.h>
#include <CQAPI.h>
#include <CQAPI_EX.h>
#include <CQLogger.h>

//���ݽṹ
#include <vector>
#include <map>
#include <string>

//ϵͳ
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <thread>

#include <boost/regex.hpp>
#include <boost/timer.hpp>

#include "../include/json/myJson.h"

using namespace std;
using namespace boost::property_tree;
static CQ::Logger logger("Ⱥ�ؼ��ʼ��");
typedef void(*Pwinfun)(const char* Path, const char* fun);


/*˽��ָ��*/
class Private
{
	enum Private_index {
		NONE,

		SEND_GROUP_1,
		SEND_GROUP_END,
		SEND_QQ,

		ADD_KEYWORD,
		ADD_KEYWORD_WHITE,
		ADD_KEYWORD_RE,

		DEL_KEYWORD,
		DEL_KEYWORD_WHITE,
		DEL_KEYWORD_RE,

		ADD_QQLIST,
		ADD_GROUPLIST_SEND,

		DEL_QQLIST,
		DEL_GROUPLIST_SEND,


		ADD_GROUPLIST,
		DEL_GROUPLIST

	};

public:
	Private(long long fromQQ = 0) :m_fromQQ(fromQQ), m_index(NONE), m_ReplyGroup(0), m_ReplyQQ(0)
	{
	}

	void put_fromQQ(long long fromQQ)
	{
		m_fromQQ = fromQQ;
	}

	//˽����Ϣ����
	void funthing(const char* msg)
	{
		std::vector<int64_t>::iterator iter;
		iter = find(g_root.begin(), g_root.end(), m_fromQQ);

		if (iter == g_root.end())
		{
			//��������ʱ����Ϣת��������
			MsgRelayFun(msg);
		}
		else
		{
			//Ϊ����ʱ����ָ���
			instruct(msg);
		}
	}

	//��Ϣת��
	void MsgRelayFun(const char* msg)
	{
		if (g_MsgRelay)
		{
			std::string SendMsg;

			//��ȡQQ����
			CQ::StrangerInfo QQInf = CQ::getStrangerInfo(m_fromQQ, true);

			//������Ϣ
			SendMsg = "����" + QQInf.nick + "(" + std::to_string(m_fromQQ) + ")��˽����Ϣ:\n\n";
			SendMsg += msg;
			SendMsg += "\n\n(�ظ��뷢�� �ظ�" + std::to_string(m_fromQQ) + ")";

			//���͸�����
			for (long long root : g_root)
			{
				CQ::sendPrivateMsg(root, SendMsg);
			}
		}
	}

	//ָ��Ի�
	void funcute(const char* msg)
	{
		switch (m_index)
		{
		case SEND_GROUP_1:                           //�ظ�Ⱥ
		{
			m_ReplyGroup = atoll(msg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢�ͻظ�����");
			m_index = SEND_GROUP_END;
			break;
		}
		case SEND_GROUP_END:                         //�ظ�Ⱥ
		{
			char ch[100] = { '\0' };
			int t;
			t = CQ::sendGroupMsg(m_ReplyGroup, msg);
			if (t > 0)
				sprintf(ch, "���͸�QQȺ%lld�ɹ�", m_ReplyGroup);
			else
				sprintf(ch, "���͸�QQȺ%lldʧ��\n������룺%d", m_ReplyGroup, t);
			CQ::sendPrivateMsg(m_fromQQ, ch);
			m_ReplyGroup = 0;
			m_index = NONE;
			break;
		}
		case ADD_KEYWORD:				        //��ӹؼ���
		{
			//֧��������� ����ÿ������
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				g_keyword[0].push_back(temp_add);

			}

			//���¹���json ��д���ļ�
			OperateFile::keyword2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "��� Ĭ�Ϲؼ��� �ɹ�");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD:						//ɾ���ؼ���
		{
			bool del_bool = false;

			auto it = g_keyword[0].begin();

			while (it != g_keyword[0].end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = g_keyword[0].erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//���¹���json ��д���ļ�
			OperateFile::keyword2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ�� Ĭ�Ϲؼ��� �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ�� Ĭ�Ϲؼ��� ʧ��");

			m_index = NONE;
			break;
		}
		case ADD_QQLIST:					//��Ӱ�����/�������
		{
			//֧��������� ����ÿ������
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				g_qqlist[0].push_back(temp_longlong);
			}

			//���¹���json ��д���ļ�
			OperateFile::qqlist2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�� ������/������� �ɹ�");

			m_index = NONE;
			break;
		}
		case DEL_QQLIST:					//ɾ��������/�������
		{
			bool del_bool = false;

			long long del_QQId = 0;
			del_QQId = atoll(msg);

			if (del_QQId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "��ʽ����");
				m_index = NONE;
				break;
			}

			auto it = g_qqlist[0].begin();

			while (it != g_qqlist[0].end())
			{
				if (*it == del_QQId)
				{
					auto del = it;

					it = g_qqlist[0].erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//���¹���json ��д���ļ�
			OperateFile::qqlist2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ������/������� �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ������/������� ʧ��");

			m_index = NONE;
			break;
		}
		case ADD_GROUPLIST:					//��Ӽ��Ⱥ
		{
			//֧��������� ����ÿ������
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				g_GroupList.push_back(temp_longlong);
			}

			//���¹���json ��д���ļ�
			OperateFile::GroupList2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "��� Ĭ�ϼ��Ⱥ �ɹ�");
			m_index = NONE;
			break;
		}
		case DEL_GROUPLIST:					//ɾ�����Ⱥ
		{
			bool del_bool = false;

			long long del_GourpId = 0;
			del_GourpId = atoll(msg);

			if (del_GourpId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "��ʽ����");
				m_index = NONE;
				break;
			}

			auto it = g_GroupList.begin();

			while (it != g_GroupList.end())
			{
				if (*it == del_GourpId)
				{
					auto del = it;

					it = g_GroupList.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//���¹���json ��д���ļ�
			OperateFile::GroupList2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ�� Ĭ�ϼ��Ⱥ �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ�� Ĭ�ϼ��Ⱥ ʧ��");
			m_index = NONE;
			break;
		}
		case SEND_QQ:                    //�ظ�QQ
		{
			char buf[100] = { '\0' };
			int t;
			t = CQ::sendPrivateMsg(m_ReplyQQ, msg);
			if (t > 0)
				sprintf(buf, "���͸� QQ%lld �ɹ�", m_ReplyQQ);
			else
				sprintf(buf, "���͸� QQ%lld ʧ��\n������룺%d", m_ReplyQQ, t);
			CQ::sendPrivateMsg(m_fromQQ, buf);
			m_ReplyQQ = 0;
			m_index = NONE;
			break;
		}//�ظ�QQ

		case ADD_KEYWORD_WHITE:                    //��Ӱ������ؼ���
		{
			//֧��������� ����ÿ������
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				g_KeyWordWhite[0].push_back(temp_add);

			}

			//���¹���json ��д���ļ�
			OperateFile::keywordWhite2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�� �������ؼ��� �ɹ�");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD_WHITE:                    //ɾ���������ؼ���
		{
			bool del_bool = false;

			auto it = g_KeyWordWhite[0].begin();

			while (it != g_KeyWordWhite[0].end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = g_KeyWordWhite[0].erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}
			//���¹���json ��д���ļ�
			OperateFile::keywordWhite2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� �������ؼ��� �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� �������ؼ��� ʧ��");

			m_index = NONE;
			break;
		}

		case ADD_KEYWORD_RE:                    //���������ʽ�ؼ���
		{
			//֧��������� ����ÿ������
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				g_REKeyWord[0].push_back(temp_add);

			}
			//���¹���json ��д���ļ�
			OperateFile::REkeyword2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�� ������ʽ�ؼ��� �ɹ�");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD_RE:                    //ɾ��������ʽ�ؼ���
		{
			bool del_bool = false;

			auto it = g_REKeyWord[0].begin();

			while (it != g_REKeyWord[0].end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = g_REKeyWord[0].erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}
			//���¹���json ��д���ļ�
			OperateFile::REkeyword2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ������ʽ�ؼ��� �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ������ʽ�ؼ��� ʧ��");

			m_index = NONE;
			break;
		}

		case ADD_GROUPLIST_SEND:                    //���ת����Ⱥ����
		{
			//֧��������� ����ÿ������
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				g_sendGroupList[0].push_back(temp_longlong);
			}

			//���¹���json ��д���ļ�
			OperateFile::sendGroupList2json();
			OperateFile::json2file();

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�� ת��Ⱥ �ɹ�");

			m_index = NONE;
			break;
		}
		case DEL_GROUPLIST_SEND:                    //ɾ��ת����Ⱥ����
		{
			bool del_bool = false;

			long long del_QQId = 0;
			del_QQId = atoll(msg);

			if (del_QQId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "��ʽ����");
				m_index = NONE;
				break;
			}

			auto it = g_sendGroupList[0].begin();

			while (it != g_sendGroupList[0].end())
			{
				if (*it == del_QQId)
				{
					auto del = it;

					it = g_sendGroupList[0].erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//���¹���json ��д���ļ�
			OperateFile::sendGroupList2json();
			OperateFile::json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ת��Ⱥ �ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�� ת��Ⱥ ʧ��");

			m_index = NONE;
			break;
		}

		}

	}

	//ָ����
	int instruct(const char* msg)
	{
		//����Ƿ���Ϊ�������ָ��Ի�
		if (m_index != NONE)
		{
			funcute(msg);
			return 0;
		}

		//���ܲ˵�
		if (!std::string(g_prefix + "����").compare(msg) ||
			!std::string(g_prefix + "�˵�").compare(msg) ||
			!strcmp(msg, "Ⱥ��ز˵�") ||
			!strcmp(msg, "Ⱥ��ع���") ||
			!strcmp(msg, "�鿴Ⱥ��ز˵�") ||
			!strcmp(msg, "�鿴Ⱥ��ع���")
			)
		{
			std::string menu =
				"**������(�Զ������ǰ׺)**\n"
				"[CQ:emoji,id=9999]�鿴/���/ɾ���ؼ���\n"
				"[CQ:emoji,id=9999]�鿴/���/ɾ���������ؼ���\n"
				"[CQ:emoji,id=9999]�鿴/���/ɾ��������ʽ�ؼ���\n"
				"[CQ:emoji,id=127984]�鿴/���/ɾ�����Ⱥ\n"
				"[CQ:emoji,id=127984]�鿴/���/ɾ��ת��Ⱥ\n"
				"[CQ:emoji,id=128203]�鿴/���/ɾ���������\n"
				"[CQ:emoji,id=128203]�鿴/���/ɾ��������\n"

				"\n**������(�Զ������ǰ׺)**\n"
				"[CQ:emoji,id=127380]�ظ�Ⱥ+QQȺ��\n"
				"[CQ:emoji,id=127380]�ظ�+QQ��\n"
				//"[CQ:emoji,id=127380]�˳�Ⱥ+QQȺ��\n"
				"\n�뷢�͹������ƣ����������飩\n"
				"��: �鿴�ؼ���";

			CQ::sendPrivateMsg(m_fromQQ, menu);
			m_index = NONE;
		}

		//��������б��Ƿ�Ϊ��
		else if (g_root.empty())
		{
			CQ::sendPrivateMsg(m_fromQQ, "����Ⱥ������ˣ�����ʹ�ý������Ⱥ�������");
			return -1;
		}

		//�ظ���
		else if (std::string(msg).find(g_prefix + "�ظ�") != string::npos)
		{
			m_ReplyQQ = atoll(msg + std::string(g_prefix + "�ظ�").length());
			if (m_ReplyQQ != 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫ�ظ�����Ϣ");
				m_index = SEND_QQ;
			}
			else
			{
				CQ::sendPrivateMsg(m_fromQQ, "���������뷢�� �ظ�+QQ��");
				m_index = NONE;
			}

		}
		else if (!std::string(g_prefix + "�ظ�Ⱥ").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫ�ظ���QQȺ��");
			m_index = SEND_GROUP_1;
		}
		else if (std::string(msg).find(g_prefix + "�ظ�Ⱥ") != string::npos)
		{
			m_ReplyGroup = atoll(msg + std::string(g_prefix + "�ظ�Ⱥ").length());

			if (m_ReplyGroup == 0)
			{
				sscanf(msg, "�ظ�Ⱥ��%lld", m_ReplyGroup);
				if (m_ReplyGroup == 0)
				{
					CQ::sendPrivateMsg(m_fromQQ, "���������밴�ո�ʽ���·���");
					return -1;
				}
			}
			CQ::sendPrivateMsg(m_fromQQ, "�뷢�ͻظ�����");
			m_index = SEND_GROUP_END;
		}


		//Ĭ�Ϲؼ���
		else if (!std::string(g_prefix + "�鿴�ؼ���").compare(msg))
		{
			string SendMsg = "ȫ��Ĭ�� �ؼ���\n";

			for (auto KeyWord : g_keyword[0])
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(g_prefix + "��ӹؼ���").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵĹؼ���(��������ӣ�ÿ��һ��)");
			m_index = ADD_KEYWORD;
		}
		else if (!std::string(g_prefix + "ɾ���ؼ���").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� �ؼ��ʣ�\n";

			for (auto WKeyWord : g_keyword[0])
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "�뷢��׼��ɾ���Ĺؼ���");
			m_index = DEL_KEYWORD;
		}

		//�������ؼ���
		else if (!std::string(g_prefix + "�鿴�������ؼ���").compare(msg))
		{
			string SendMsg = "ȫ��Ĭ�� �������ؼ��ʣ�\n";

			for (auto KeyWord : g_KeyWordWhite[0])
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(g_prefix + "��Ӱ������ؼ���").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵ� �������ؼ��� (��������ӣ�ÿ��һ��)");
			m_index = ADD_KEYWORD_WHITE;
		}
		else if (!std::string(g_prefix + "ɾ���������ؼ���").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� �������ؼ��ʣ�\n";

			for (auto WKeyWord : g_KeyWordWhite[0])
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "�뷢��׼��ɾ���� �������ؼ���");
			m_index = DEL_KEYWORD_WHITE;
		}

		//������ʽ�ؼ���
		else if (!std::string(g_prefix + "�鿴������ʽ�ؼ���").compare(msg))
		{
			string SendMsg = "ȫ��Ĭ�� ������ʽ�ؼ��ʣ�\n";

			for (auto KeyWord : g_REKeyWord[0])
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(g_prefix + "���������ʽ�ؼ���").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵ� ������ʽ�ؼ��� (��������ӣ�ÿ��һ��)");
			m_index = ADD_KEYWORD_RE;
		}
		else if (!std::string(g_prefix + "ɾ��������ʽ�ؼ���").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� ������ʽ�ؼ��ʣ�\n";

			for (auto WKeyWord : g_REKeyWord[0])
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "�뷢��׼��ɾ���� ������ʽ�ؼ���");
			m_index = DEL_KEYWORD_RE;
		}

		//����ת��Ⱥ
		else if (!std::string(g_prefix + "�鿴ת��Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� ת��Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_sendGroupList[0])
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}


			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = NONE;
		}
		else if (!std::string(g_prefix + "���ת��Ⱥ").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵ� Ⱥ���� (��������ӣ�ÿ��һ��)");
			m_index = ADD_GROUPLIST_SEND;
		}
		else if (!std::string(g_prefix + "ɾ��ת��Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� ת��Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_sendGroupList[0])
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫɾ�� Ⱥ����");
			m_index = DEL_GROUPLIST_SEND;
		}

		//���ü��Ⱥ
		else if (!std::string(g_prefix + "��Ӽ��Ⱥ").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵ�Ⱥ����(��������ӣ�ÿ��һ��)");
			m_index = ADD_GROUPLIST;
		}
		else if (!std::string(g_prefix + "ɾ�����Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� ���Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_GroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫɾ��Ⱥ����");
			m_index = DEL_GROUPLIST;
		}
		else if (!std::string(g_prefix + "�鿴���Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�� ���Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_GroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}


			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = NONE;
		}

		//���ð�����
		else if (!std::string(g_prefix + "��Ӱ�����").compare(msg) || !std::string(g_prefix + "��Ӽ������").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�˴���δ�Ż��������в鿴�Ƿ�����Ϊ����Ҫ�ļ��ģʽ(������/�������)");

			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵİ�����/�������QQ(��������ӣ�ÿ��һ��)");
			m_index = ADD_QQLIST;
		}
		else if (!std::string(g_prefix + "ɾ��������").compare(msg) || !std::string(g_prefix + "ɾ���������").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�˴���δ�Ż��������в鿴�Ƿ�����Ϊ����Ҫ�ļ��ģʽ(������/�������)");

			std::string SendMsg = "ȫ��Ĭ�� ������/���������\n";

			for (auto temp : g_qqlist[0])
			{
				auto QQ_inf = CQ::getStrangerInfo(temp);
				SendMsg += QQ_inf.nick + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫɾ������������");
			m_index = DEL_QQLIST;
		}
		else if (!std::string(g_prefix + "�鿴������").compare(msg) || !std::string(g_prefix + "�鿴�������").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�˴���δ�Ż��������в鿴�Ƿ�����Ϊ����Ҫ�ļ��ģʽ(������/�������)");

			std::string SendMsg = "ȫ��Ĭ�� ������/���������\n";

			for (auto temp : g_qqlist[0])
			{
				auto QQ_inf = CQ::getStrangerInfo(temp);
				SendMsg += QQ_inf.nick + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = NONE;
		}


		return m_index;
	}

private:
	Private_index m_index;//ָ��λ��
	long long m_ReplyGroup;//��Ҫ�ظ���Ⱥ����
	long long m_ReplyQQ;//��Ҫ�ظ���QQ����
	long long m_fromQQ;//��Ϣ���Ե�QQ����

};


/*��Ϣ����*/

class OperateMsg
{
public:
	//�����ؼ��ʲ���
	void KeyWrodWarn(stringstream& SendMsg_root, int conf_index, string& KeyWord, string REWord = "")
	{

		//ɾ�������CQ��
		DelCQ(m_msg);

		string sendMsg_toGroup(g_conf[conf_index].sendGroupMsg_word);

		//��������
		sendGroupMsgWord(sendMsg_toGroup, conf_index, KeyWord);

		//���� Ⱥ��Ϣ
		for (long long GroupId : g_sendGroupList[conf_index])
		{
			CQ::sendGroupMsg(GroupId, sendMsg_toGroup);
		}

		//���� ˽����Ϣ

		std::string GroupAt;//Ⱥ����

		if (m_fromQQ == 80000000)
		{
			SendMsg_root << "����Ⱥ" << m_fromGroup << +"��\n";
			SendMsg_root << "QQ����:" << m_fromQQ << "(QQ����)" << "\n";
			SendMsg_root << "QQ����:" << "������Ϣ" << "\n";
			SendMsg_root << "QQȺ��Ƭ:" << "������Ϣ" << "\n";
			SendMsg_root << "��������:\n\n" << m_msg << "\n";
		}
		else
		{
			CQ::GroupMemberInfo QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);

			SendMsg_root << "����Ⱥ" << m_fromGroup << +"��\n";
			SendMsg_root << "QQ����:" << m_fromQQ << "\n";
			SendMsg_root << "QQ����:" << QQInf.�ǳ� << "\n";
			SendMsg_root << "QQȺ��Ƭ:" << QQInf.��Ƭ << "\n";
			SendMsg_root << "��������:\n\n" << m_msg << "\n";
		}

		if (!REWord.empty())
		{
			SendMsg_root << "\n������ʽ:" << REWord << "\n";
		}

		SendMsg_root << "\n�����˹ؼ���:" << KeyWord << "\n";

		if (g_conf[conf_index].Revoke)
			CQ::deleteMsg(m_msgId);

		if (m_fromQQ == 80000000)
		{
			SendMsg_root << "��������QQ�������ݲ�����������";

			GroupAt = "@���� �����˹ؼ�����";
		}
		else
		{
			if (g_conf[conf_index].way == 1)
			{
				if (m_fromQQ == 80000000)
					CQ::setGroupAnonymousBan(m_fromGroup, m_fromAnonymous.c_str(), g_conf[conf_index].wayTime * 60);
				else
					CQ::setGroupBan(m_fromGroup, m_fromQQ, g_conf[conf_index].wayTime * 60);

				std::string TimeStr;

				int TimeDay = 0;
				int TimeHour = 0;
				int TimeMin = 0;

				TimeDay = g_conf[conf_index].wayTime / 1440;
				TimeHour = (g_conf[conf_index].wayTime - TimeDay * 1440) / 60;
				TimeMin = g_conf[conf_index].wayTime % 60;

				if (TimeDay)
				{
					TimeStr = std::to_string(TimeDay) + "��";
				}

				if (TimeHour)
				{
					TimeStr += std::to_string(TimeHour) + "Сʱ";
				}

				if (TimeMin)
				{
					TimeStr += std::to_string(TimeMin) + "����";
				}

				SendMsg_root << "������������" + TimeStr;

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]�����˹ؼ�����ѱ�����" + TimeStr;

			}
			else if (g_conf[conf_index].way == 2)
			{
				CQ::setGroupKick(m_fromGroup, m_fromQQ);

				SendMsg_root << "���������߳�";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]�����˹ؼ�����ѱ��߳���Ⱥ";
			}
			else if (g_conf[conf_index].way == 3)
			{
				CQ::setGroupKick(m_fromGroup, m_fromQQ, 1);

				SendMsg_root << "���������߳�������";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]�����˹ؼ�����ѱ��߳���Ⱥ������";
			}
			else
			{
				SendMsg_root << "��������δ������";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]�����˹ؼ�����";
			}
		}

		if (g_conf[conf_index].GroupWarn)
		{

			if (!g_conf[conf_index].WordKeyWarn.empty())
			{
				string SendMsg(g_conf[conf_index].WordKeyWarn);

				//��������
				KeyWordWarnMsg(SendMsg, conf_index, KeyWord);

				CQ::sendGroupMsg(m_fromGroup, SendMsg);
			}
			else
			{
				CQ::sendGroupMsg(m_fromGroup, GroupAt);
			}
		}

		SendMsg_root << "\n���δ����ܺ�ʱ:";
		SendMsg_root << m_time.elapsed() << "��";
		SendMsg_root << "\n(�ظ��뷢�ͣ��ظ�Ⱥ" << m_fromGroup << ")";

		for (long long root : g_root)
		{
			CQ::sendPrivateMsg(root, SendMsg_root.str().c_str());
		}

	}

	//ɾ��cq��
	void DelCQ(std::string& msg)
	{
		//��ʱ ֻɾ��Url

		if (msg.find("[CQ:share,url=") != string::npos)
		{
			boost::regex e1("\\[CQ:share,url=");
			msg = boost::regex_replace(msg, e1, "");

			e1 = ",title=.*\\]";
			msg = boost::regex_replace(msg, e1, "");

		}
	}

	//ת����Ⱥ��ʽ
	void sendGroupMsgWord(std::string& str, int conf_index, string keyword)
	{

		//Ĭ������Ϊmsg
		if (str.empty())
		{
			str = "{msg}";
		}

		//��Ϣ����
		if (str.find("{msg}") != string::npos)
		{
			string temp_msg(m_msg);
			int frontLine = 0;
			int afterLine = temp_msg.size();

			int i = 0;

			if (g_conf[conf_index].sendGroupMsg_frontLine)
			{
				for (auto it = temp_msg.begin(); it != temp_msg.end(); it++)
				{
					frontLine++;
					if (*it == '\n')
					{
						i++;
						if (i == g_conf[conf_index].sendGroupMsg_frontLine)
							break;
					}
				}
			}

			if (g_conf[conf_index].sendGroupMsg_afterLine)
			{
				i = 0;
				for (auto it = temp_msg.rbegin(); it != temp_msg.rend(); it++)
				{
					afterLine--;
					if (*it == '\n')
					{
						i++;
						if (i == g_conf[conf_index].sendGroupMsg_afterLine)
							break;
					}
				}
			}

			temp_msg[afterLine] = 0;
			temp_msg = temp_msg.c_str() + frontLine;

			//�����Ϣ��ʽ�Ƿ����
			if (temp_msg.empty())
			{
				string sendMsg;
				sendMsg = "��������Ϣ��ʽ����\n��Ϣ����:\n\n";
				sendMsg += m_msg;
				sendMsg += "\n\n��鿴ת����Ⱥ��ʽ��ɾ�����Ƿ�����";

				for (auto temp : g_root)
				{
					CQ::sendPrivateMsg(temp, sendMsg);
				}
				str = "";

				return;
			}


			str = OperateStr::replace_all_distinct(str, "{msg}", temp_msg);

		}

		//��ȡ��ǰʱ��
		SYSTEMTIME sys;
		GetLocalTime(&sys);

		char date[20];
		char time[20];
		char Week[20];

		sprintf(date, "%4d��%02d��%02d��", sys.wYear, sys.wMonth, sys.wDay);
		sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		sprintf(Week, "����%d", sys.wDayOfWeek);

		//����
		str = OperateStr::replace_all_distinct(str, "{����}", date);

		//ʱ��
		str = OperateStr::replace_all_distinct(str, "{ʱ��}", time);

		//����
		str = OperateStr::replace_all_distinct(str, "{����}", Week);

		//�����Ĺؼ���
		str = OperateStr::replace_all_distinct(str, "{�ؼ���}", keyword);

		//������QQ����
		str = OperateStr::replace_all_distinct(str, "{������QQ����}", to_string(m_fromQQ));


		auto QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);
		auto GroupInf = CQ::getGroupList();
		//������QQ����
		str = OperateStr::replace_all_distinct(str, "{������QQ����}", QQInf.�ǳ�);

		//������QQ��Ƭ
		str = OperateStr::replace_all_distinct(str, "{������QQ��Ƭ}", QQInf.��Ƭ);

		//������Ⱥ����
		str = OperateStr::replace_all_distinct(str, "{������Ⱥ����}", GroupInf[m_fromGroup]);

	}

	//�Զ��崥���ؼ�������
	void KeyWordWarnMsg(std::string& str, int conf_index, string keyword)
	{
		//����
		str = OperateStr::replace_all_distinct(str, "{at}", "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]");

		//��ȡ��ǰʱ��
		SYSTEMTIME sys;
		GetLocalTime(&sys);

		char date[20];
		char time[20];
		char Week[20];

		sprintf(date, "%4d��%02d��%02d��", sys.wYear, sys.wMonth, sys.wDay);
		sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		sprintf(Week, "����%d", sys.wDayOfWeek);

		//����
		str = OperateStr::replace_all_distinct(str, "{����}", date);

		//ʱ��
		str = OperateStr::replace_all_distinct(str, "{ʱ��}", time);

		//����
		str = OperateStr::replace_all_distinct(str, "{����}", Week);

		//����ʽ
		if (g_conf[conf_index].way == 1)
		{
			std::string TimeStr = "����";

			int TimeDay = 0;
			int TimeHour = 0;
			int TimeMin = 0;

			TimeDay = g_conf[conf_index].wayTime / 1440;
			TimeHour = (g_conf[conf_index].wayTime - TimeDay * 1440) / 60;
			TimeMin = g_conf[conf_index].wayTime % 60;

			if (TimeDay)
			{
				TimeStr += std::to_string(TimeDay) + "��";
			}

			if (TimeHour)
			{
				TimeStr += std::to_string(TimeHour) + "Сʱ";
			}

			if (TimeMin)
			{
				TimeStr += std::to_string(TimeMin) + "����";
			}


			str = OperateStr::replace_all_distinct(str, "{����ʽ}", TimeStr);
		}
		else if (g_conf[conf_index].way == 2)
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "�߳���Ⱥ");
		}
		else if (g_conf[conf_index].way == 3)
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "�߳�������");
		}
		else
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "δ������");
		}

		//�����Ĺؼ���
		str = OperateStr::replace_all_distinct(str, "{�ؼ���}", keyword);

		//������QQ����
		str = OperateStr::replace_all_distinct(str, "{������QQ����}", to_string(m_fromQQ));


		auto QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);
		auto GroupInf = CQ::getGroupList();
		//������QQ����
		str = OperateStr::replace_all_distinct(str, "{������QQ����}", QQInf.�ǳ�);

		//������QQ��Ƭ
		str = OperateStr::replace_all_distinct(str, "{������QQ��Ƭ}", QQInf.��Ƭ);

		//������Ⱥ����
		str = OperateStr::replace_all_distinct(str, "{������Ⱥ����}", GroupInf[m_fromGroup]);

	}

	//�������ؼ��ʼ��
	bool KeyWordWhiteFun(vector<WKEYWORD>& KeyWordWhite)
	{
		if (KeyWordWhite.empty())
			return false;

		//���ҹؼ��� �������ĳһ�� �򷵻�true
		for (WKEYWORD KeyWord_one : KeyWordWhite)
		{
			if (m_wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos)
			{

				return true;
			}

		}

		return false;
	}

	//�ؼ��ʶԱ�  KeyWord�ؼ������� Strengǿ����� KeyWordWarn���ش����Ĺؼ���
	bool KeyWordFun(int conf_index, string* KeyWordWarn)
	{
		//�жϹؼ����Ƿ�Ϊ��
		if (g_keyword[conf_index].empty())
		{
			return false;
		}

		//��ͨ�ؼ��ʼ��
		for (auto KeyWord_one : g_keyword[conf_index])
		{
			if (m_wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos)
			{
				//���ش����Ĺؼ���
				if (KeyWordWarn != NULL)
					*KeyWordWarn = KeyWord_one.keyWord;

				return true;
			}
		}

		//ǿ�����
		if (g_conf[conf_index].Streng)
		{

			for (auto KeyWord_one : g_keyword[conf_index])
			{
				//��¼�ؼ���������ƥ����
				int temp_num = KeyWord_one.wkeyWrod.size(), temp_num_find = 0;

				//�ж���Ϣ�͹ؼ����Ƿ�Ϊ��
				if (KeyWord_one.wkeyWrod.empty() || m_wmsg.empty())
				{
					return false;
				}

				//��ʼǿ�����   ����ֶԱ�
				for (auto keyc : KeyWord_one.wkeyWrod)
				{
					for (auto msgc : m_wmsg)
					{
						if (keyc == msgc)
						{
							temp_num_find++;
							break;
						}
					}
				}


				//����ǿ������
				if (temp_num_find >= temp_num)
				{
					//���ش����Ĺؼ���
					if (KeyWordWarn != NULL)
						*KeyWordWarn = KeyWord_one.keyWord;

					return true;
				}
			}
		}

		return false;
	}

	//������ʽ  REKeyWord ������ʽ�ؼ��� REKeyWordWarn���ش��������� REWord���ش����Ĺؼ���
	bool REKeyKordFun(int conf_index, string* REKeyWordWarn, string* REWord)
	{
		for (WKEYWORD alone : g_REKeyWord[conf_index])
		{
			try {

				boost::wregex re(alone.wkeyWrod);
				boost::wsmatch RE;

				bool rec = boost::regex_search(m_wmsg, RE, re);

				if (rec)
				{
					*REKeyWordWarn = OperateStr::wstring2string(RE.str());
					*REWord = alone.keyWord;

					return true;
				}

			}
			catch (exception & e)
			{
				string SendMsg;
				SendMsg += "������ʽ����\n";
				SendMsg += "���ʽ:";
				SendMsg += alone.keyWord;
				SendMsg += "��Ϣ:\n\n";
				SendMsg += m_msg;
				SendMsg += "exception���صĴ�����Ϣ:";
				SendMsg += e.what();

				for (long long root : g_root)
				{
					CQ::sendPrivateMsg(root, SendMsg);
				}
			}
		}
		return false;
	}

	//���ݴ�����
	void MsgOneFun(int conf_index)
	{
		//���Ϊ��������ģʽ
		if (g_conf[conf_index].typeQQList == 0)
		{
			//����Ƿ�Ϊ������ ���Ϊ���������˳�����
			if (find(g_qqlist[conf_index].begin(), g_qqlist[conf_index].end(), m_fromQQ) != g_qqlist[conf_index].end())
			{
				return;
			}
		}
		//���Ϊ���������ģʽ
		else if (g_conf[conf_index].typeQQList == 1)
		{
			//����ڼ������û�ҵ��˺��� ���˳�����
			if (find(g_qqlist[conf_index].begin(), g_qqlist[conf_index].end(), m_fromQQ) == g_qqlist[conf_index].end())
			{
				return;
			}
		}

		//����ؼ���Ϊ������ ���˳�����
		if (KeyWordWhiteFun(g_KeyWordWhite[0]))
		{
			return;
		}


		string KeyWordWarn;

		//��������˹ؼ���
		if (KeyWordFun(conf_index, &KeyWordWarn))
		{
			stringstream SendMsg_root("�����˹ؼ���\n");
			KeyWrodWarn(SendMsg_root, conf_index, KeyWordWarn);

			return;
		}

		string REWord;
		//���������������ʽ�ؼ���
		if (REKeyKordFun(conf_index, &KeyWordWarn, &REWord))
		{
			stringstream SendMsg_root("������������ʽ\n");
			KeyWrodWarn(SendMsg_root, conf_index, KeyWordWarn, REWord);

			return;
		}
	}

	//���ݴ���
	void MsgFun()
	{
		m_time.restart();
		//�����Ƿ�ΪĬ�ϼ��Ⱥ
		auto list_bool = find(g_GroupList.begin(), g_GroupList.end(), m_fromGroup);

		//����ҵ��� ΪĬ�ϼ��Ⱥ
		if (list_bool != g_GroupList.end())
		{
			MsgOneFun(0);//ִ��Ĭ�ϴ���ʽ
		}

		//�����Ƿ�Ϊ��������Ⱥ
		list_bool = find(g_AlGroupList.begin(), g_AlGroupList.end(), m_fromGroup);
		//���Ⱥ��������ִ��
		if (list_bool != g_AlGroupList.end())
		{
			MsgOneFun(m_fromGroup);//ִ�е�������ʽ
		}


	}

	OperateMsg(int64_t fromQQ, string fromAnonymous, int64_t fromGroup, int64_t msgId, const char* msg) :m_fromQQ(fromQQ), m_fromAnonymous(fromAnonymous), m_fromGroup(fromGroup), m_msg(msg), m_msgId(msgId)
	{
		//����Ϣ�Ŀ��ַ�����ʽ��ŵ�������
		m_wmsg = OperateStr::string2wstring(msg);
	}

private:
	int64_t m_fromQQ;//���Ե�QQ����
	int64_t m_fromGroup;//���Ե�Ⱥ��
	string m_fromAnonymous;//��Դ��������Ϣ
	string m_msg;//��Ϣ����
	int64_t m_msgId;//��Ϣid
	boost::timer m_time;//���������ʧ��ʱ��

	wstring m_wmsg;//���ֽ���Ϣ����

};

//Ⱥ�б����
void CQ_getGroupList_charp(string& str)
{
	ptree json_temp;

	map<long long, std::string> a = CQ::getGroupList();

	for (auto temp : a)
	{
		json_temp.put<string>(to_string(temp.first).c_str(), temp.second);
	}

	std::stringstream s2;
	write_json(s2, json_temp);
	str = s2.str();
}



Pwinfun win;  //��dll�е���Ĵ򿪽��溯��

EVE_Menu(__menu)
{
	//OperateFile::all2json();
	//OperateFile::json2file();
	static bool is_open = false;

	if (is_open)
	{
		MessageBoxA(NULL, "ϵͳ��⵽��һ�����ý���δ�ر�,�볢��������Q�����´����ý���", "����", NULL);
		//logger.Info("�����ʧ��");
		return -1;
	}


	is_open = true;
	string str;
	CQ_getGroupList_charp(str);


	if (win == NULL)
	{
		is_open = false;
		MessageBoxA(NULL, "�����ʧ�ܣ���������û�и��ƽ���dll\n�����⻶ӭ��QQȺ��839067703 ����", "����", NULL);
		//logger.Info("�����ʧ��");
		return -1;
	}

	win(AppFile.c_str(), str.c_str());
	//�޸����ú����¶�ȡ����
	OperateFile::file2json();
	OperateFile::json2all();

	for (long long root : g_root)
	{
		CQ::sendPrivateMsg(root, "��Ⱥ�ؼ��ʼ�ء���������Ч");
	}

	is_open = false;

	return 0;
}