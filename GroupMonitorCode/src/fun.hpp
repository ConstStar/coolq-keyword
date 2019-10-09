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



/*˽��ָ��*/
class Private
{

public:
	Private(long long fromQQ = 0) :m_fromQQ(fromQQ), m_index(0), m_ReplyGroup(0), m_ReplyQQ(0)
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
			CQ::StrangerInfo QQInf = CQ::getStrangerInfo(m_fromQQ);

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
		case 5:
		{
			m_ReplyGroup = atoll(msg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢�ͻظ�����");
			m_index = 6;
			break;
		}
		case 6:
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
			m_index = 0;
			break;
		}
		case 7:				//��ӹؼ���
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

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�Ϲؼ��ʳɹ�");

			m_index = 0;
			break;
		}
		case 8:						//ɾ���ؼ���
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
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�Ϲؼ��ʳɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�Ϲؼ���ʧ��");

			m_index = 0;
			break;
		}
		case 11:					//��Ӱ�����/�������
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

			m_index = 0;
			break;
		}
		case 12:					//ɾ��������/�������
		{
			bool del_bool = false;

			long long del_QQId = 0;
			del_QQId = atoll(msg);

			if (del_QQId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "��ʽ����");
				m_index = 0;
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

			m_index = 0;
			break;
		}
		case 13:					//��Ӽ��Ⱥ
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

			CQ::sendPrivateMsg(m_fromQQ, "���Ĭ�ϼ��Ⱥ�ɹ�");
			m_index = 0;
			break;
		}
		case 14:					//ɾ�����Ⱥ
		{
			bool del_bool = false;

			long long del_GourpId = 0;
			del_GourpId = atoll(msg);

			if (del_GourpId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "��ʽ����");
				m_index = 0;
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
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�ϼ��Ⱥ�ɹ�");
			else
				CQ::sendPrivateMsg(m_fromQQ, "ɾ��Ĭ�ϼ��Ⱥʧ��");
			m_index = 0;
			break;
		}
		case 15:
		{
			char buf[100] = { '\0' };
			int t;
			t = CQ::sendPrivateMsg(m_ReplyQQ, msg);
			if (t > 0)
				sprintf(buf, "���͸�QQ%lld�ɹ�", m_ReplyQQ);
			else
				sprintf(buf, "���͸�QQ%lldʧ��\n������룺%d", m_ReplyQQ, t);
			CQ::sendPrivateMsg(m_fromQQ, buf);
			m_ReplyQQ = 0;
			m_index = 0;
			break;
		}//�ظ�QQ
		}

	}

	//ָ����
	int instruct(const char* msg)
	{
		if (m_index != 0)
		{
			funcute(msg);
			return 0;
		}

		if (!strcmp(msg, "Ⱥ��ز˵�") || !strcmp(msg, "Ⱥ��ع���") || !strcmp(msg, "�鿴Ⱥ��ز˵�") || !strcmp(msg, "�鿴Ⱥ��ع���"))
		{
			std::string menu =
				"**������(�Զ������ǰ׺)**\n"
				"[CQ:emoji,id=9999]��ӹؼ���\n"
				"[CQ:emoji,id=9999]�鿴�ؼ���\n"
				"[CQ:emoji,id=9999]ɾ���ؼ���\n"
				"[CQ:emoji,id=127984]��Ӽ��Ⱥ\n"
				"[CQ:emoji,id=127984]ɾ�����Ⱥ\n"
				"[CQ:emoji,id=127984]�鿴���Ⱥ\n"
				"[CQ:emoji,id=128203]��Ӱ�����\n"
				"[CQ:emoji,id=128203]ɾ��������\n"
				"[CQ:emoji,id=128203]�鿴������\n"
				"\n**������(�Զ������ǰ׺)**\n"
				"[CQ:emoji,id=127380]�ظ�Ⱥ+QQȺ��\n"
				"[CQ:emoji,id=127380]�ظ�+QQ��\n"
				//"[CQ:emoji,id=127380]�˳�Ⱥ+QQȺ��\n"
				"�뷢�͹������ƣ����������飩";
			CQ::sendPrivateMsg(m_fromQQ, menu);
			m_index = 0;
		}

		else if (!std::string(g_prefix + "�鿴�ؼ���").compare(msg))
		{
			string SendMsg = "ȫ������Ĭ�Ϲؼ���\n";

			for (auto KeyWord : g_keyword[0])
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = 0;
		}

		else if (g_root.empty())
		{
			CQ::sendPrivateMsg(m_fromQQ, "����Ⱥ������ˣ�����ʹ�ý������Ⱥ�������");
			return -1;
		}

		else if (!std::string(g_prefix + "��Ӱ�����").compare(msg))
		{

			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵİ�����/�������QQ(��������ӣ�ÿ��һ��)");
			m_index = 11;
		}
		else if (!std::string(g_prefix + "ɾ��������").compare(msg) || !std::string(g_prefix + "ɾ���������").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�ϰ�����/���������\n";

			for (auto temp : g_qqlist[0])
			{
				auto QQ_inf = CQ::getStrangerInfo(temp);
				SendMsg += QQ_inf.nick + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫɾ������������");
			m_index = 12;
		}
		else if (!std::string(g_prefix + "�鿴������").compare(msg) || !std::string(g_prefix + "�鿴�������").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�ϰ�����/���������\n";

			for (auto temp : g_qqlist[0])
			{
				auto QQ_inf = CQ::getStrangerInfo(temp);
				SendMsg += QQ_inf.nick + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = 0;
		}
		else if (!std::string(g_prefix + "��Ӽ��Ⱥ").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵ�Ⱥ����(��������ӣ�ÿ��һ��)");
			m_index = 13;
		}
		else if (!std::string(g_prefix + "ɾ�����Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�ϼ��Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_GroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫɾ��Ⱥ����");
			m_index = 14;
		}
		else if (!std::string(g_prefix + "�鿴���Ⱥ").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�ϼ��Ⱥ��\n";
			auto temp_GourpList = CQ::getGroupList();

			//�г����Ⱥ����
			for (auto temp : g_GroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}


			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = 0;
		}
		else if (!std::string(g_prefix + "�ظ�Ⱥ").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫ�ظ���QQȺ��");
			m_index = 5;
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
			m_index = 6;
		}
		else if (!std::string(g_prefix + "��ӹؼ���").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "�뷢����ӵĹؼ���(��������ӣ�ÿ��һ��)");
			m_index = 7;
		}
		else if (!std::string(g_prefix + "ɾ���ؼ���").compare(msg))
		{
			std::string SendMsg = "ȫ��Ĭ�Ϲؼ��ʣ�\n";

			for (auto WKeyWord : g_keyword[0])
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "�뷢��׼��ɾ���Ĺؼ���");
			m_index = 8;
		}

		else if (std::string(msg).find(g_prefix + "�ظ�") != string::npos)
		{
			m_ReplyQQ = atoll(msg + std::string(g_prefix + "�ظ�").length());
			if (m_ReplyQQ != 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "�뷢��Ҫ�ظ�����Ϣ");
				m_index = 15;
			}
			else
			{
				CQ::sendPrivateMsg(m_fromQQ, "���������뷢�� �ظ�+QQ��");
				m_index = 0;
			}

		}
		return m_index;
	}

private:
	int m_index;//ָ��λ��
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
		sendGroupMsgWord(sendMsg_toGroup, conf_index);

		//���� Ⱥ��Ϣ
		for (long long GroupId : g_sendGroupList[conf_index])
		{
			CQ::sendGroupMsg(GroupId, sendMsg_toGroup);
		}


		//���� ˽����Ϣ

		CQ::GroupMemberInfo QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ);

		std::string GroupAt;//Ⱥ����

		SendMsg_root << "����Ⱥ" << m_fromGroup << +"��\n";
		SendMsg_root << "QQ����:" << m_fromQQ << "\n";
		SendMsg_root << "QQ����:" << QQInf.�ǳ� << "\n";
		SendMsg_root << "QQȺ��Ƭ:" << QQInf.��Ƭ << "\n";
		SendMsg_root << "��������:\n\n" << m_msg << "\n";


		if (!REWord.empty())
		{
			SendMsg_root << "\n������ʽ:" << REWord;
		}

		SendMsg_root << "\n�����˹ؼ���:" << KeyWord << "\n";

		if (g_conf[conf_index].Revoke)
			CQ::deleteMsg(m_msgId);

		if (g_conf[conf_index].way == 1)
		{
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

			SendMsg_root <<"������������" + TimeStr;

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

		if (g_conf[conf_index].GroupWarn)
		{

			if (!g_conf[conf_index].WordKeyWarn.empty())
			{
				string SendMsg(g_conf[conf_index].WordKeyWarn);

				//��������
				KeyWordWarnMsg(SendMsg, g_conf[conf_index].way, g_conf[conf_index].wayTime);

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
	void sendGroupMsgWord(std::string& str, int conf_index)
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


	}

	//�Զ��崥���ؼ�������
	void KeyWordWarnMsg(std::string& str, int Way, int WayTime)
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
		if (Way == 1)
		{
			std::string TimeStr = "����";

			int TimeDay = 0;
			int TimeHour = 0;
			int TimeMin = 0;

			TimeDay = WayTime / 1440;
			TimeHour = (WayTime - TimeDay * 1440) / 60;
			TimeMin = WayTime % 60;

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
		else if (Way == 2)
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "�߳���Ⱥ");
		}
		else if (Way == 3)
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "�߳�������");
		}
		else
		{
			str = OperateStr::replace_all_distinct(str, "{����ʽ}", "δ������");
		}

	}

	/*
	////����������
	//bool AloneFun(string alone_name, Json::Value alone)
	//{

	//	string KeyWordWarn;

	//	if (alone["Streng"].isBool())
	//		alone["Streng"] = false;

	//	if (alone["Way"].isInt())
	//		alone["Way"] = 0;

	//	if (alone["WayTime"].isInt())
	//		alone["WayTime"] = 0;

	//	if (alone["Revoke"].isBool())
	//		alone["Revoke"] = false;

	//	if (alone["GroupWarn"].isBool())
	//		alone["GroupWarn"] = false;

	//	if (alone["WordKeyWarn"].isString())
	//		alone["WordKeyWarn"] = "";

	//	if (alone["REKeyWord"].isString())
	//		alone["REKeyWord"] = "";


	//	//��������˹ؼ���
	//	if (KeyWordFun(alone["wkeyword"], alone["Streng"].asBool(), &KeyWordWarn))
	//	{
	//		string SendMsg_root = "�����˵��������еġ�" + alone_name + "��\n";
	//		KeyWrodWarn(
	//			SendMsg_root,
	//			alone["Way"].asInt(),
	//			alone["WayTime"].asInt(),
	//			alone["Revoke"].asBool(),
	//			alone["GroupWarn"].asBool(),
	//			alone["WordKeyWarn"].asString(),
	//			KeyWordWarn);

	//		return true;
	//	}

	//	string REWrod;
	//	//���������������ʽ
	//	if (REKeyKordFun(alone["REKeyWord"], &KeyWordWarn, &REWrod))
	//	{
	//		string SendMsg_root = "�����˵��������еġ�" + alone_name + "��������ʽ\n";

	//		KeyWrodWarn(
	//			SendMsg_root,
	//			alone["Way"].asInt(),
	//			alone["WayTime"].asInt(),
	//			alone["Revoke"].asBool(),
	//			alone["GroupWarn"].asBool(),
	//			alone["WordKeyWarn"].asString(),
	//			KeyWordWarn,
	//			REWrod);
	//		return true;
	//	}

	//	return false;
	//}

	*/

	//�������ؼ��ʼ��
	bool KeyWordWhiteFun(vector<string>& KeyWordWhite)
	{
		if (KeyWordWhite.empty())
			return false;

		//���ҹؼ��� �������ĳһ�� �򷵻�true
		for (auto KeyWord_one : KeyWordWhite)
		{
			if (m_msg.find(KeyWord_one) != std::wstring::npos)
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
		for (auto alone : g_REKeyWord[conf_index])
		{
			try {

				boost::regex re(alone);
				boost::smatch RE;

				bool rec = boost::regex_search(m_msg, RE, re);

				if (rec)
				{
					*REKeyWordWarn = RE.str().c_str();
					*REWord = alone;

					return true;
				}

			}
			catch (exception & e)
			{
				string SendMsg;
				SendMsg += "������ʽ����\n";
				SendMsg += "���ʽ:";
				SendMsg += alone;
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

	OperateMsg(int64_t fromQQ, int64_t fromGroup, int64_t msgId, const char* msg) :m_fromQQ(fromQQ), m_fromGroup(fromGroup), m_msg(msg), m_msgId(msgId)
	{
		//����Ϣ�Ŀ��ַ�����ʽ��ŵ�������
		m_wmsg = OperateStr::string2wstring(msg);
	}

private:
	int64_t m_fromQQ;//���Ե�QQ����
	int64_t m_fromGroup;//���Ե�Ⱥ��
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

EVE_Menu(__menu)
{
	//OperateFile::all2json();
	//OperateFile::json2file();

	string str;
	CQ_getGroupList_charp(str);
	//CQ_get(*GroupList)_fun(fun);


	/*DllFun a;
	a.get(*GroupList) = get(*GroupList);*/
	typedef void(*Pwinfun)(const char* Path, const char* fun);
	HINSTANCE hDLL;
	Pwinfun win;
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //���ض�̬���ӿ�MyDll.dll�ļ���
	if (hDLL == NULL) {
		MessageBoxA(NULL, "�����ʧ�ܣ���������û�и��ƽ���dll\n�����⻶ӭ��QQȺ��839067703 ����", "����", NULL);
		//logger.Info("�����ʧ��");
		return -1;
	}
	win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	win(AppFile.c_str(), str.c_str());

	FreeLibrary(hDLL);
	//�޸����ú����¶�ȡ����
	OperateFile::file2json();
	OperateFile::json2all();

	for (long long root : g_root)
	{
		CQ::sendPrivateMsg(root, "��Ⱥ�ؼ��ʼ�ء���������Ч");
	}

	return 0;
}