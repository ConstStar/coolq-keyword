#pragma once
#include <CQSDK.h>

//数据结构
#include <vector>
#include <map>
#include <string>

//系统
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <thread>

#include <boost/regex.hpp>
#include <boost/timer.hpp>

#include "myJson.h"

using namespace std;


static CQ::Logger logger("群关键词监控");

string appDir;
MyJson conf;

/*私聊指令*/
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

	//私聊消息处理
	void funthing(const char* msg)
	{
		std::vector<int64_t>::iterator iter;
		iter = find(conf.admin.begin(), conf.admin.end(), m_fromQQ);

		if (iter == conf.admin.end())
		{
			//不是主人时将消息转发给主人
			MsgRelayFun(msg);
		}
		else
		{
			//为主人时进入指令函数
			instruct(msg);
		}
	}

	//消息转发
	void MsgRelayFun(const char* msg)
	{
		if (conf.msgRelay)
		{
			std::string SendMsg;

			//获取QQ资料
			CQ::StrangerInfo QQInf = CQ::getStrangerInfo(m_fromQQ, true);

			//构造消息
			SendMsg = "来自" + QQInf.nick + "(" + std::to_string(m_fromQQ) + ")的私聊消息:\n\n";
			SendMsg += msg;
			SendMsg += "\n\n(回复请发送 回复" + std::to_string(m_fromQQ) + ")";

			//发送给主人
			for (long long root : conf.admin)
			{
				CQ::sendPrivateMsg(root, SendMsg);
			}
		}
	}

	//指令对话
	void funcute(const char* msg)
	{
		switch (m_index)
		{
		case SEND_GROUP_1:                           //回复群
		{
			m_ReplyGroup = atoll(msg);
			CQ::sendPrivateMsg(m_fromQQ, "请发送回复内容");
			m_index = SEND_GROUP_END;
			break;
		}
		case SEND_GROUP_END:                         //回复群
		{
			char ch[100] = { '\0' };
			int t;
			t = CQ::sendGroupMsg(m_ReplyGroup, msg);
			if (t > 0)
				sprintf(ch, "发送给QQ群%lld成功", m_ReplyGroup);
			else
				sprintf(ch, "发送给QQ群%lld失败\n错误代码：%d", m_ReplyGroup, t);
			CQ::sendPrivateMsg(m_fromQQ, ch);
			m_ReplyGroup = 0;
			m_index = NONE;
			break;
		}
		case ADD_KEYWORD:				        //添加关键词
		{
			//支持批量添加 解析每行数据
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				conf.alone[0].keyWord.push_back(temp_add);

			}

			//重新构造json 并写入文件
			conf.keyword2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加 默认关键词 成功");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD:						//删除关键词
		{
			bool del_bool = false;

			auto it = conf.alone[0].keyWord.begin();

			while (it != conf.alone[0].keyWord.end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = conf.alone[0].keyWord.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//重新构造json 并写入文件
			conf.keyword2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除 默认关键词 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除 默认关键词 失败");

			m_index = NONE;
			break;
		}
		case ADD_QQLIST:					//添加白名单/监控名单
		{
			//支持批量添加 解析每行数据
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				conf.alone[0].QQList.push_back(temp_longlong);
			}

			//重新构造json 并写入文件
			conf.qqlist2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加默认 白名单/监控名单 成功");

			m_index = NONE;
			break;
		}
		case DEL_QQLIST:					//删除白名单/监控名单
		{
			bool del_bool = false;

			long long del_QQId = 0;
			del_QQId = atoll(msg);

			if (del_QQId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "格式有误");
				m_index = NONE;
				break;
			}

			auto it = conf.alone[0].QQList.begin();

			while (it != conf.alone[0].QQList.end())
			{
				if (*it == del_QQId)
				{
					auto del = it;

					it = conf.alone[0].QQList.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//重新构造json 并写入文件
			conf.qqlist2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 白名单/监控名单 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 白名单/监控名单 失败");

			m_index = NONE;
			break;
		}
		case ADD_GROUPLIST:					//添加监控群
		{
			//支持批量添加 解析每行数据
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				conf.alone[0].groupList.push_back(temp_longlong);
			}

			//重新构造json 并写入文件
			conf.GroupList2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加 默认监控群 成功");
			m_index = NONE;
			break;
		}
		case DEL_GROUPLIST:					//删除监控群
		{
			bool del_bool = false;

			long long del_GourpId = 0;
			del_GourpId = atoll(msg);

			if (del_GourpId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "格式有误");
				m_index = NONE;
				break;
			}

			auto it = conf.alone[0].groupList.begin();

			while (it != conf.alone[0].groupList.end())
			{
				if (*it == del_GourpId)
				{
					auto del = it;

					it = conf.alone[0].groupList.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//重新构造json 并写入文件
			conf.GroupList2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除 默认监控群 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除 默认监控群 失败");
			m_index = NONE;
			break;
		}
		case SEND_QQ:                    //回复QQ
		{
			char buf[100] = { '\0' };
			int t;
			t = CQ::sendPrivateMsg(m_ReplyQQ, msg);
			if (t > 0)
				sprintf(buf, "发送给 QQ%lld 成功", m_ReplyQQ);
			else
				sprintf(buf, "发送给 QQ%lld 失败\n错误代码：%d", m_ReplyQQ, t);
			CQ::sendPrivateMsg(m_fromQQ, buf);
			m_ReplyQQ = 0;
			m_index = NONE;
			break;
		}//回复QQ

		case ADD_KEYWORD_WHITE:                    //添加白名单关键词
		{
			//支持批量添加 解析每行数据
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				conf.alone[0].keyWordWhite.push_back(temp_add);

			}

			//重新构造json 并写入文件
			conf.keywordWhite2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加默认 白名单关键词 成功");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD_WHITE:                    //删除白名单关键词
		{
			bool del_bool = false;

			auto it = conf.alone[0].keyWordWhite.begin();

			while (it != conf.alone[0].keyWordWhite.end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = conf.alone[0].keyWordWhite.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}
			//重新构造json 并写入文件
			conf.keywordWhite2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 白名单关键词 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 白名单关键词 失败");

			m_index = NONE;
			break;
		}

		case ADD_KEYWORD_RE:                    //添加正则表达式关键词
		{
			//支持批量添加 解析每行数据
			vector<string> temp_vector;
			OperateLine::line_get_str(msg, temp_vector);

			for (auto temp_keyword : temp_vector)
			{
				WKEYWORD temp_add(temp_keyword);
				conf.alone[0].keyWordRegex.push_back(temp_add);

			}
			//重新构造json 并写入文件
			conf.REkeyword2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加默认 正则表达式关键词 成功");

			m_index = NONE;
			break;
		}
		case DEL_KEYWORD_RE:                    //删除正则表达式关键词
		{
			bool del_bool = false;

			auto it = conf.alone[0].keyWordRegex.begin();

			while (it != conf.alone[0].keyWordRegex.end())
			{
				if (it->keyWord == msg)
				{
					auto del = it;

					it = conf.alone[0].keyWordRegex.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}
			//重新构造json 并写入文件
			conf.REkeyword2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 正则表达式关键词 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 正则表达式关键词 失败");

			m_index = NONE;
			break;
		}

		case ADD_GROUPLIST_SEND:                    //添加转发到群号码
		{
			//支持批量添加 解析每行数据
			vector<long long> temp_vector;
			OperateLine::line_get_ll(msg, temp_vector);

			for (auto temp_longlong : temp_vector)
			{
				conf.alone[0].relayGroupList.push_back(temp_longlong);
			}

			//重新构造json 并写入文件
			conf.sendGroupList2json();
			conf.json2file();

			CQ::sendPrivateMsg(m_fromQQ, "添加默认 转发群 成功");

			m_index = NONE;
			break;
		}
		case DEL_GROUPLIST_SEND:                    //删除转发到群号码
		{
			bool del_bool = false;

			long long del_QQId = 0;
			del_QQId = atoll(msg);

			if (del_QQId == 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "格式有误");
				m_index = NONE;
				break;
			}

			auto it = conf.alone[0].relayGroupList.begin();

			while (it != conf.alone[0].relayGroupList.end())
			{
				if (*it == del_QQId)
				{
					auto del = it;

					it = conf.alone[0].relayGroupList.erase(del);
					del_bool = true;
				}
				else
				{
					it++;
				}

			}

			//重新构造json 并写入文件
			conf.sendGroupList2json();
			conf.json2file();

			if (del_bool)
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 转发群 成功");
			else
				CQ::sendPrivateMsg(m_fromQQ, "删除默认 转发群 失败");

			m_index = NONE;
			break;
		}

		}

	}

	//指令检测
	int instruct(const char* msg)
	{
		string& prefix = conf.prefix;


		//检测是否有为处理完的指令对话
		if (m_index != NONE)
		{
			funcute(msg);
			return 0;
		}

		//功能菜单
		if (!std::string(prefix + "功能").compare(msg) ||
			!std::string(prefix + "菜单").compare(msg) ||
			!strcmp(msg, "群监控菜单") ||
			!strcmp(msg, "群监控功能") ||
			!strcmp(msg, "查看群监控菜单") ||
			!strcmp(msg, "查看群监控功能")
			)
		{
			std::string menu =
				"**设置类(自定义需加前缀)**\n"
				"[CQ:emoji,id=9999]查看/添加/删除关键词\n"
				"[CQ:emoji,id=9999]查看/添加/删除白名单关键词\n"
				"[CQ:emoji,id=9999]查看/添加/删除正则表达式关键词\n"
				"[CQ:emoji,id=127984]查看/添加/删除监控群\n"
				"[CQ:emoji,id=127984]查看/添加/删除转发群\n"
				"[CQ:emoji,id=128203]查看/添加/删除监控名单\n"
				"[CQ:emoji,id=128203]查看/添加/删除白名单\n"

				"\n**其他类(自定义需加前缀)**\n"
				"[CQ:emoji,id=127380]回复群+QQ群号\n"
				"[CQ:emoji,id=127380]回复+QQ号\n"
				//"[CQ:emoji,id=127380]退出群+QQ群号\n"
				"\n请发送功能名称（不包括表情）\n"
				"如: 查看关键词";

			CQ::sendPrivateMsg(m_fromQQ, menu);
			m_index = NONE;
		}

		//检测主人列表是否为空
		else if (conf.admin.empty())
		{
			CQ::sendPrivateMsg(m_fromQQ, "暂无群监控主人，请先使用界面添加群监控主人");
			return -1;
		}

		//回复类
		else if (std::string(msg).find(prefix + "回复") != string::npos)
		{
			m_ReplyQQ = atoll(msg + std::string(prefix + "回复").length());
			if (m_ReplyQQ != 0)
			{
				CQ::sendPrivateMsg(m_fromQQ, "请发送要回复的消息");
				m_index = SEND_QQ;
			}
			else
			{
				CQ::sendPrivateMsg(m_fromQQ, "输入有误，请发送 回复+QQ号");
				m_index = NONE;
			}

		}
		else if (!std::string(prefix + "回复群").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送要回复的QQ群号");
			m_index = SEND_GROUP_1;
		}
		else if (std::string(msg).find(prefix + "回复群") != string::npos)
		{
			m_ReplyGroup = atoll(msg + std::string(prefix + "回复群").length());

			if (m_ReplyGroup == 0)
			{
				sscanf(msg, "回复群＋%lld", m_ReplyGroup);
				if (m_ReplyGroup == 0)
				{
					CQ::sendPrivateMsg(m_fromQQ, "输入有误，请按照格式重新发送");
					return -1;
				}
			}
			CQ::sendPrivateMsg(m_fromQQ, "请发送回复内容");
			m_index = SEND_GROUP_END;
		}


		//默认关键词
		else if (!std::string(prefix + "查看关键词").compare(msg))
		{
			string SendMsg = "全局默认 关键词\n";

			for (auto KeyWord : conf.alone[0].keyWord)
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(prefix + "添加关键词").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的关键词(可批量添加，每行一个)");
			m_index = ADD_KEYWORD;
		}
		else if (!std::string(prefix + "删除关键词").compare(msg))
		{
			std::string SendMsg = "全局默认 关键词：\n";

			for (auto WKeyWord : conf.alone[0].keyWord)
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "请发送准备删除的关键词");
			m_index = DEL_KEYWORD;
		}

		//白名单关键词
		else if (!std::string(prefix + "查看白名单关键词").compare(msg))
		{
			string SendMsg = "全局默认 白名单关键词：\n";

			for (auto KeyWord : conf.alone[0].keyWordWhite)
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(prefix + "添加白名单关键词").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的 白名单关键词 (可批量添加，每行一个)");
			m_index = ADD_KEYWORD_WHITE;
		}
		else if (!std::string(prefix + "删除白名单关键词").compare(msg))
		{
			std::string SendMsg = "全局默认 白名单关键词：\n";

			for (auto WKeyWord : conf.alone[0].keyWordWhite)
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "请发送准备删除的 白名单关键词");
			m_index = DEL_KEYWORD_WHITE;
		}

		//正则表达式关键词
		else if (!std::string(prefix + "查看正则表达式关键词").compare(msg))
		{
			string SendMsg = "全局默认 正则表达式关键词：\n";

			for (auto KeyWord : conf.alone[0].keyWordRegex)
				SendMsg += KeyWord.keyWord + "\n";

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			m_index = NONE;
		}
		else if (!std::string(prefix + "添加正则表达式关键词").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的 正则表达式关键词 (可批量添加，每行一个)");
			m_index = ADD_KEYWORD_RE;
		}
		else if (!std::string(prefix + "删除正则表达式关键词").compare(msg))
		{
			std::string SendMsg = "全局默认 正则表达式关键词：\n";

			for (auto WKeyWord : conf.alone[0].keyWordRegex)
			{
				SendMsg += WKeyWord.keyWord + "\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			CQ::sendPrivateMsg(m_fromQQ, "请发送准备删除的 正则表达式关键词");
			m_index = DEL_KEYWORD_RE;
		}

		//设置转发群
		else if (!std::string(prefix + "查看转发群").compare(msg))
		{
			std::string SendMsg = "全局默认 转发群：\n";
			auto temp_GourpList = CQ::getGroupList();

			//列出监控群名单
			for (auto temp : conf.alone[0].relayGroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}


			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = NONE;
		}
		else if (!std::string(prefix + "添加转发群").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的 群号码 (可批量添加，每行一个)");
			m_index = ADD_GROUPLIST_SEND;
		}
		else if (!std::string(prefix + "删除转发群").compare(msg))
		{
			std::string SendMsg = "全局默认 转发群：\n";
			auto temp_GourpList = CQ::getGroupList();

			//列出监控群名单
			for (auto temp : conf.alone[0].relayGroupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "请发送要删除 群号码");
			m_index = DEL_GROUPLIST_SEND;
		}

		//设置监控群
		else if (!std::string(prefix + "添加监控群").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的群号码(可批量添加，每行一个)");
			m_index = ADD_GROUPLIST;
		}
		else if (!std::string(prefix + "删除监控群").compare(msg))
		{
			std::string SendMsg = "全局默认 监控群：\n";
			auto temp_GourpList = CQ::getGroupList();

			//列出监控群名单
			for (auto temp : conf.alone[0].groupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "请发送要删除群号码");
			m_index = DEL_GROUPLIST;
		}
		else if (!std::string(prefix + "查看监控群").compare(msg))
		{
			std::string SendMsg = "全局默认 监控群：\n";
			auto temp_GourpList = CQ::getGroupList();

			//列出监控群名单
			for (auto temp : conf.alone[0].groupList)
			{
				SendMsg += temp_GourpList[temp] + "(" + to_string(temp) + ")\n";
			}


			CQ::sendPrivateMsg(m_fromQQ, SendMsg);

			m_index = NONE;
		}

		//设置白名单
		else if (!std::string(prefix + "添加白名单").compare(msg) || !std::string(prefix + "添加监控名单").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "此处暂未优化，请自行查看是否设置为您需要的监控模式(白名单/监控名单)");

			CQ::sendPrivateMsg(m_fromQQ, "请发送添加的白名单/监控名单QQ(可批量添加，每行一个)");
			m_index = ADD_QQLIST;
		}
		else if (!std::string(prefix + "删除白名单").compare(msg) || !std::string(prefix + "删除监控名单").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "此处暂未优化，请自行查看是否设置为您需要的监控模式(白名单/监控名单)");

			std::string SendMsg = "全局默认 白名单/监控名单：\n";

			for (auto temp : conf.alone[0].QQList)
			{
				auto QQ_inf = CQ::getStrangerInfo(temp);
				SendMsg += QQ_inf.nick + "(" + to_string(temp) + ")\n";
			}

			CQ::sendPrivateMsg(m_fromQQ, SendMsg);
			CQ::sendPrivateMsg(m_fromQQ, "请发送要删除白名单号码");
			m_index = DEL_QQLIST;
		}
		else if (!std::string(prefix + "查看白名单").compare(msg) || !std::string(prefix + "查看监控名单").compare(msg))
		{
			CQ::sendPrivateMsg(m_fromQQ, "此处暂未优化，请自行查看是否设置为您需要的监控模式(白名单/监控名单)");

			std::string SendMsg = "全局默认 白名单/监控名单：\n";

			for (auto temp : conf.alone[0].QQList)
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
	Private_index m_index;//指令位置

	long long m_ReplyGroup;//需要回复的群号码
	long long m_ReplyQQ;//需要回复的QQ号码
	long long m_fromQQ;//消息来自的QQ号码

};


/*消息处理*/

class OperateMsg
{
public:
	//触发关键词操作
	void KeyWrodWarn(stringstream& SendMsg_root, int conf_index, string& KeyWord, string REWord = "")
	{

		//删除多余的CQ码
		DelCQ(m_msg);

		string sendMsg_toGroup(conf.alone[conf_index].relayGroupWord);

		//解析变量
		sendGroupMsgWord(sendMsg_toGroup, conf_index, KeyWord);

		//发送 群消息
		for (long long GroupId : conf.alone[conf_index].relayGroupList)
		{
			CQ::sendGroupMsg(GroupId, sendMsg_toGroup);
		}

		//发送 私聊消息

		std::string GroupAt;//群艾特

		if (m_fromQQ == 80000000)
		{
			SendMsg_root << "来自群" << m_fromGroup << +"中\n";
			SendMsg_root << "QQ号码:" << m_fromQQ << "(QQ匿名)" << "\n";
			SendMsg_root << "QQ名称:" << "匿名消息" << "\n";
			SendMsg_root << "QQ群名片:" << "匿名消息" << "\n";
			SendMsg_root << "由于内容:\n\n" << m_msg << "\n";
		}
		else
		{
			CQ::GroupMemberInfo QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);

			SendMsg_root << "来自群" << m_fromGroup << +"中\n";
			SendMsg_root << "QQ号码:" << m_fromQQ << "\n";
			SendMsg_root << "QQ名称:" << QQInf.昵称 << "\n";
			SendMsg_root << "QQ群名片:" << QQInf.名片 << "\n";
			SendMsg_root << "由于内容:\n\n" << m_msg << "\n";
		}

		if (!REWord.empty())
		{
			SendMsg_root << "\n正则表达式:" << REWord << "\n";
		}

		SendMsg_root << "\n触发了关键词:" << KeyWord << "\n";

		if (conf.alone[conf_index].revoke)
			CQ::deleteMsg(m_msgId);

		if (m_fromQQ == 80000000)
		{
			SendMsg_root << "处理方法：QQ匿名，暂不能做出处理";

			GroupAt = "@匿名 触发了关键词语";
		}
		else
		{
			int dealType = conf.alone[conf_index].dealType;
			if (dealType == 1)
			{
				if (m_fromQQ == 80000000)
					CQ::setGroupAnonymousBan(m_fromGroup, m_fromAnonymous.c_str(), conf.alone[conf_index].banTimeLen * 60);
				else
					CQ::setGroupBan(m_fromGroup, m_fromQQ, conf.alone[conf_index].banTimeLen * 60);

				std::string TimeStr;

				int TimeDay = 0;
				int TimeHour = 0;
				int TimeMin = 0;

				TimeDay = conf.alone[conf_index].banTimeLen / 1440;
				TimeHour = (conf.alone[conf_index].banTimeLen - TimeDay * 1440) / 60;
				TimeMin = conf.alone[conf_index].banTimeLen % 60;

				if (TimeDay)
				{
					TimeStr = std::to_string(TimeDay) + "天";
				}

				if (TimeHour)
				{
					TimeStr += std::to_string(TimeHour) + "小时";
				}

				if (TimeMin)
				{
					TimeStr += std::to_string(TimeMin) + "分钟";
				}

				SendMsg_root << "处理方法：禁言" + TimeStr;

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]触发了关键词语，已被禁言" + TimeStr;

			}
			else if (dealType == 2)
			{
				CQ::setGroupKick(m_fromGroup, m_fromQQ);

				SendMsg_root << "处理方法：踢出";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]触发了关键词语，已被踢出本群";
			}
			else if (dealType == 3)
			{
				CQ::setGroupKick(m_fromGroup, m_fromQQ, 1);

				SendMsg_root << "处理方法：踢出并拉黑";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]触发了关键词语，已被踢出本群并拉黑";
			}
			else
			{
				SendMsg_root << "处理方法：未做处理";

				GroupAt = "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]触发了关键词语";
			}
		}

		if (conf.alone[conf_index].groupWarn)
		{

			if (!conf.alone[conf_index].keyWordGroupWarn.empty())
			{
				string SendMsg(conf.alone[conf_index].keyWordGroupWarn);

				//解析变量
				KeyWordWarnMsg(SendMsg, conf_index, KeyWord);

				CQ::sendGroupMsg(m_fromGroup, SendMsg);
			}
			else
			{
				CQ::sendGroupMsg(m_fromGroup, GroupAt);
			}
		}

		SendMsg_root << "\n本次处理总耗时:";
		SendMsg_root << m_time.elapsed() << "秒";
		SendMsg_root << "\n(回复请发送：回复群" << m_fromGroup << ")";

		for (long long root : conf.admin)
		{
			CQ::sendPrivateMsg(root, SendMsg_root.str().c_str());
		}

	}

	//删除cq码
	void DelCQ(std::string& msg)
	{
		//暂时 只删除Url

		if (msg.find("[CQ:share,url=") != string::npos)
		{
			boost::regex e1("\\[CQ:share,url=");
			msg = boost::regex_replace(msg, e1, "");

			e1 = ",title=.*\\]";
			msg = boost::regex_replace(msg, e1, "");

		}
	}

	//转发到群格式
	void sendGroupMsgWord(std::string& str, int conf_index, string keyword)
	{

		//默认内容为msg
		if (str.empty())
		{
			str = "{msg}";
		}

		//消息内容
		if (str.find("{msg}") != string::npos)
		{
			string temp_msg(m_msg);
			int frontLine = 0;
			int afterLine = temp_msg.size();

			int i = 0;

			if (conf.alone[conf_index].relayGroupMsg_frontLine)
			{
				for (auto it = temp_msg.begin(); it != temp_msg.end(); it++)
				{
					frontLine++;
					if (*it == '\n')
					{
						i++;
						if (i == conf.alone[conf_index].relayGroupMsg_frontLine)
							break;
					}
				}
			}

			if (conf.alone[conf_index].relayGroupMsg_afterLine)
			{
				i = 0;
				for (auto it = temp_msg.rbegin(); it != temp_msg.rend(); it++)
				{
					afterLine--;
					if (*it == '\n')
					{
						i++;
						if (i == conf.alone[conf_index].relayGroupMsg_afterLine)
							break;
					}
				}
			}

			temp_msg[afterLine] = 0;
			temp_msg = temp_msg.c_str() + frontLine;

			//检测消息格式是否错误
			if (temp_msg.empty())
			{
				string sendMsg;
				sendMsg = "解析后消息格式错误\n消息内容:\n\n";
				sendMsg += m_msg;
				sendMsg += "\n\n请查看转发到群格式中删除行是否有误";

				for (auto temp : conf.admin)
				{
					CQ::sendPrivateMsg(temp, sendMsg);
				}
				str = "";

				return;
			}


			str = OperateStr::replace_all_distinct(str, "{msg}", temp_msg);

		}

		//获取当前时间
		SYSTEMTIME sys;
		GetLocalTime(&sys);

		char date[20];
		char time[20];
		char Week[20];

		sprintf(date, "%4d年%02d月%02d日", sys.wYear, sys.wMonth, sys.wDay);
		sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		sprintf(Week, "星期%d", sys.wDayOfWeek);

		//日期
		str = OperateStr::replace_all_distinct(str, "{日期}", date);

		//时间
		str = OperateStr::replace_all_distinct(str, "{时间}", time);

		//星期
		str = OperateStr::replace_all_distinct(str, "{星期}", Week);

		//触发的关键词
		str = OperateStr::replace_all_distinct(str, "{关键词}", keyword);

		//触发的QQ号码
		str = OperateStr::replace_all_distinct(str, "{触发的QQ号码}", to_string(m_fromQQ));


		auto QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);
		auto GroupInf = CQ::getGroupList();
		//触发的QQ名称
		str = OperateStr::replace_all_distinct(str, "{触发的QQ名称}", QQInf.昵称);

		//触发的QQ名片
		str = OperateStr::replace_all_distinct(str, "{触发的QQ名片}", QQInf.名片);

		//触发的群名称
		str = OperateStr::replace_all_distinct(str, "{触发的群名称}", GroupInf[m_fromGroup]);

	}

	//自定义触发关键词提醒
	void KeyWordWarnMsg(std::string& str, int conf_index, string keyword)
	{
		//艾特
		str = OperateStr::replace_all_distinct(str, "{at}", "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]");

		//获取当前时间
		SYSTEMTIME sys;
		GetLocalTime(&sys);

		char date[20];
		char time[20];
		char Week[20];

		sprintf(date, "%4d年%02d月%02d日", sys.wYear, sys.wMonth, sys.wDay);
		sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		sprintf(Week, "星期%d", sys.wDayOfWeek);

		//日期
		str = OperateStr::replace_all_distinct(str, "{日期}", date);

		//时间
		str = OperateStr::replace_all_distinct(str, "{时间}", time);

		//星期
		str = OperateStr::replace_all_distinct(str, "{星期}", Week);

		//处理方式
		int dealType = conf.alone[conf_index].dealType;
		if (dealType == 1)
		{
			std::string TimeStr = "禁言";

			int banTimeLen = conf.alone[conf_index].banTimeLen;

			int TimeDay = 0;
			int TimeHour = 0;
			int TimeMin = 0;

			TimeDay = banTimeLen / 1440;
			TimeHour = (banTimeLen - TimeDay * 1440) / 60;
			TimeMin = banTimeLen % 60;

			if (TimeDay)
			{
				TimeStr += std::to_string(TimeDay) + "天";
			}

			if (TimeHour)
			{
				TimeStr += std::to_string(TimeHour) + "小时";
			}

			if (TimeMin)
			{
				TimeStr += std::to_string(TimeMin) + "分钟";
			}


			str = OperateStr::replace_all_distinct(str, "{处理方式}", TimeStr);
		}
		else if (dealType == 2)
		{
			str = OperateStr::replace_all_distinct(str, "{处理方式}", "踢出本群");
		}
		else if (dealType == 3)
		{
			str = OperateStr::replace_all_distinct(str, "{处理方式}", "踢出并拉黑");
		}
		else
		{
			str = OperateStr::replace_all_distinct(str, "{处理方式}", "未做处理");
		}

		//触发的关键词
		str = OperateStr::replace_all_distinct(str, "{关键词}", keyword);

		//触发的QQ号码
		str = OperateStr::replace_all_distinct(str, "{触发的QQ号码}", to_string(m_fromQQ));


		auto QQInf = CQ::getGroupMemberInfo(m_fromGroup, m_fromQQ, true);
		auto GroupInf = CQ::getGroupList();
		//触发的QQ名称
		str = OperateStr::replace_all_distinct(str, "{触发的QQ名称}", QQInf.昵称);

		//触发的QQ名片
		str = OperateStr::replace_all_distinct(str, "{触发的QQ名片}", QQInf.名片);

		//触发的群名称
		str = OperateStr::replace_all_distinct(str, "{触发的群名称}", GroupInf[m_fromGroup]);

	}

	//白名单关键词检测
	bool KeyWordWhiteFun(vector<WKEYWORD>& KeyWordWhite)
	{
		if (KeyWordWhite.empty())
			return false;

		//查找关键词 如果存在某一个 则返回true
		for (WKEYWORD KeyWord_one : KeyWordWhite)
		{
			if (m_wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos)
			{

				return true;
			}

		}

		return false;
	}

	//关键词对比  KeyWord关键词容器 Streng强力检测 KeyWordWarn返回触发的关键词
	bool KeyWordFun(int conf_index, string* KeyWordWarn)
	{
		//判断关键词是否为空
		if (conf.alone[conf_index].keyWord.empty())
		{
			return false;
		}

		//普通关键词检测
		for (auto KeyWord_one : conf.alone[conf_index].keyWord)
		{
			if (m_wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos)
			{
				//返回触发的关键词
				if (KeyWordWarn != NULL)
					*KeyWordWarn = KeyWord_one.keyWord;

				return true;
			}
		}

		//强力检测
		if (conf.alone[conf_index].streng)
		{

			for (auto KeyWord_one : conf.alone[conf_index].keyWord)
			{
				//记录关键词字数与匹配数
				int temp_num = KeyWord_one.wkeyWrod.size(), temp_num_find = 0;

				//判断消息和关键词是否为空
				if (KeyWord_one.wkeyWrod.empty() || m_wmsg.empty())
				{
					return false;
				}

				//开始强力检测   逐个字对比
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


				//触发强力检测后
				if (temp_num_find >= temp_num)
				{
					//返回触发的关键词
					if (KeyWordWarn != NULL)
						*KeyWordWarn = KeyWord_one.keyWord;

					return true;
				}
			}
		}

		return false;
	}

	//正则表达式  REKeyWord 正则表达式关键词 REKeyWordWarn返回触发的内容 REWord返回触发的关键词
	bool REKeyKordFun(int conf_index, string* REKeyWordWarn, string* REWord)
	{
		for (WKEYWORD alone : conf.alone[conf_index].keyWordRegex)
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
				SendMsg += "正则表达式崩溃\n";
				SendMsg += "表达式:";
				SendMsg += alone.keyWord;
				SendMsg += "消息:\n\n";
				SendMsg += m_msg;
				SendMsg += "exception返回的错误消息:";
				SendMsg += e.what();

				for (long long root : conf.admin)
				{
					CQ::sendPrivateMsg(root, SendMsg);
				}
			}
		}
		return false;
	}

	//内容处理功能
	void MsgOneFun(int conf_index)
	{
		//如果为检测白名单模式
		if (conf.alone[conf_index].QQListType == 0)
		{
			//检测是否为白名单 如果为白名单将退出函数
			if (find(conf.alone[conf_index].QQList.begin(), conf.alone[conf_index].QQList.end(), m_fromQQ) != conf.alone[conf_index].QQList.end())
			{
				return;
			}
		}
		//如果为检测监控名单模式
		else if (conf.alone[conf_index].QQListType == 1)
		{
			//如果在监控名单没找到此号码 则退出函数
			if (find(conf.alone[conf_index].QQList.begin(), conf.alone[conf_index].QQList.end(), m_fromQQ) == conf.alone[conf_index].QQList.end())
			{
				return;
			}
		}

		//如果关键词为白名单 将退出函数
		if (KeyWordWhiteFun(conf.alone[0].keyWordWhite))
		{
			return;
		}


		string KeyWordWarn;

		//如果触发了关键词
		if (KeyWordFun(conf_index, &KeyWordWarn))
		{
			stringstream SendMsg_root("触发了关键词\n");
			KeyWrodWarn(SendMsg_root, conf_index, KeyWordWarn);

			return;
		}

		string REWord;
		//如果触发了正则表达式关键词
		if (REKeyKordFun(conf_index, &KeyWordWarn, &REWord))
		{
			stringstream SendMsg_root("触发了正则表达式\n");
			KeyWrodWarn(SendMsg_root, conf_index, KeyWordWarn, REWord);

			return;
		}
	}

	//内容处理
	void MsgFun()
	{
		m_time.restart();
		//查找是否为默认监控群
		auto list_bool = find(conf.alone[0].groupList.begin(), conf.alone[0].groupList.end(), m_fromGroup);

		//如果找到看 为默认监控群
		if (list_bool != conf.alone[0].groupList.end())
		{
			MsgOneFun(0);//执行默认处理方式
		}

		////查找是否为单独设置群
		//list_bool = find(g_AlGroupList.begin(), g_AlGroupList.end(), m_fromGroup);
		////如果群单独开启执行
		//if (list_bool != g_AlGroupList.end())
		//{
		//	MsgOneFun(m_fromGroup);//执行单独处理方式
		//}


	}

	OperateMsg(int64_t fromQQ, string fromAnonymous, int64_t fromGroup, int64_t msgId, const char* msg) :m_fromQQ(fromQQ), m_fromAnonymous(fromAnonymous), m_fromGroup(fromGroup), m_msg(msg), m_msgId(msgId)
	{
		//将消息的宽字符串格式存放到对象中
		m_wmsg = OperateStr::string2wstring(msg);
	}

private:
	int64_t m_fromQQ;//来自的QQ号码
	int64_t m_fromGroup;//来自的群号
	string m_fromAnonymous;//来源的匿名信息
	string m_msg;//消息内容
	int64_t m_msgId;//消息id
	boost::timer m_time;//计算程序流失的时间

	wstring m_wmsg;//宽字节消息内容

};