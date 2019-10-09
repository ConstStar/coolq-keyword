#pragma once
#include "cqdefine.h"
#include "CQEVEBasic.h"
#include "CQMsgSend.h"

#include <string>
#include <map>
namespace CQ {
	// 字体
	struct Font {
		文本型 名称;
		整数型 字号,
			颜色,
			/*粗体：1 斜体：2 下划线：4*/
			样式,
			气泡;

		explicit Font(int);
	};
	//正则消息
	class regexMsg {
		//消息
		std::map<std::string, std::string> regexMap;
	public:
		regexMsg(std::string msg);
		std::string get(std::string);
		std::string operator [](std::string);
	};
	class MsgSend;
	//消息事件基类
	struct EVEMsg :public EVE
	{
		//子类型
		int subType;
		//消息ID
		int msgId;
		//来源QQ
		long long fromQQ;
		//消息
		std::string message;
		char* messageRAW;
		//字体
		//Font font;
		int font;

		EVEMsg(int subType, int msgId, long long fromQQ, const char* message, int font);

		//真实用户
		bool isUser() const;
		//是否是系统用户
		bool isSystem() const;

		virtual int sendMsg(const char*) const = 0;
		virtual int sendMsg(std::string) const = 0;
		virtual MsgSend sendMsg() const = 0;
	};
}