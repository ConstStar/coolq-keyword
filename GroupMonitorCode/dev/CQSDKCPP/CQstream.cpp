/*此文件是下面三个头文件的实现*/
#include "..\..\inculde\bufstream.h"
#include "..\..\inculde\CQLogger.h"
#include "..\..\inculde\CQMsgSend.h"

#include "..\..\inculde\CQAPI_EX.h"
#include "..\..\inculde\CQTools.h"

using namespace CQ;
using namespace std;

CQ::Logger::Logger(std::string title) : title(title) {}

void CQ::Logger::setTitle(std::string title) { this->title = title; }

void CQ::Logger::Debug(std::string & msg) const { Debug(msg.c_str()); }

void CQ::Logger::Info(std::string & msg) const { Info(msg.c_str()); }

void CQ::Logger::InfoSuccess(std::string & msg) const { InfoSuccess(msg.c_str()); }

void CQ::Logger::InfoRecv(std::string & msg) const { InfoRecv(msg.c_str()); }

void CQ::Logger::InfoSend(std::string & msg) const { InfoSend(msg.c_str()); }

void CQ::Logger::Warning(std::string & msg) const { Warning(msg.c_str()); }

void CQ::Logger::Error(std::string & msg) const { Error(msg.c_str()); }

void CQ::Logger::Fatal(std::string & msg) const { Fatal(msg.c_str()); }

void CQ::Logger::Debug(const char * msg) const { addLog(Log_Debug, title.c_str(), msg); }

void CQ::Logger::Info(const char * msg) const { addLog(Log_Info, title.c_str(), msg); }

void CQ::Logger::InfoSuccess(const char * msg) const { addLog(Log_InfoSuccess, title.c_str(), msg); }

void CQ::Logger::InfoRecv(const char * msg) const { addLog(Log_InfoRecv, title.c_str(), msg); }

void CQ::Logger::InfoSend(const char * msg) const { addLog(Log_InfoSend, title.c_str(), msg); }

void CQ::Logger::Warning(const char * msg) const { addLog(Log_Warning, title.c_str(), msg); }

void CQ::Logger::Error(const char * msg) const { addLog(Log_Error, title.c_str(), msg); }

void CQ::Logger::Fatal(const char * msg) const { addLog(Log_Fatal, title.c_str(), msg); }

logstream CQ::Logger::Debug() const { return logstream(title, Log_Debug); }

logstream CQ::Logger::Info() const { return logstream(title, Log_Info); }

logstream CQ::Logger::InfoSuccess() const { return logstream(title, Log_InfoSuccess); }

logstream CQ::Logger::InfoRecv() const { return logstream(title, Log_InfoRecv); }

logstream CQ::Logger::InfoSend() const { return logstream(title, Log_InfoSend); }

logstream CQ::Logger::Warning() const { return logstream(title, Log_Warning); }

logstream CQ::Logger::Error() const { return logstream(title, Log_Error); }

logstream CQ::Logger::Fatal() const { return logstream(title, Log_Fatal); }

void CQ::send(CQstream & log) { log.SEND(); log.clear(); }
void CQ::flush(CQstream & log) { log.FLUSH(); }
void CQ::endl(CQstream & log) { log << "\r\n"; }
void CQ::RAW(CQstream & log) { log.raw = true; }

void CQ::CODE(CQstream & log) { log.raw = false; }

bool CQ::CQstream::isRAW() { return raw; }

void CQ::CQstream::clear() { buf.clear(); }

CQstream & CQ::CQstream::append(const string & s) { buf += (raw ? msg_encode(string(s)) : s); return *this; }

CQstream & CQ::CQstream::operator<<(const string & s) { return (*this).append(s); }

CQstream & CQ::CQstream::append(const char * s) { buf += (raw ? msg_encode(string(s)) : s); return *this; }

CQstream & CQ::CQstream::operator<<(const char * c) { return (*this).append(c); }

CQstream & CQ::CQstream::append(const bool & i) { buf += i ? "true" : "false"; return *this; }

CQstream & CQ::CQstream::operator<<(const bool & i) { return (*this).append(i); }

CQstream & CQ::CQstream::append(const int & i) { buf += to_string(i); return *this; }

CQstream & CQ::CQstream::operator<<(const int & i) { return (*this).append(i); }

CQstream & CQ::CQstream::append(const size_t & i) { buf += to_string(i); return *this; }

CQstream & CQ::CQstream::operator<<(const size_t & i) { return (*this).append(i); }

CQstream & CQ::CQstream::append(const double & i) { buf += to_string(i); return *this; }

CQstream & CQ::CQstream::operator<<(const double & i) { return (*this).append(i); }

CQstream & CQ::CQstream::append(const long & i) { buf += to_string(i); return *this; }

CQstream & CQ::CQstream::operator<<(const long & i) { return (*this).append(i); }

CQstream & CQ::CQstream::append(const long long & l) { buf += to_string(l); return *this; }

CQstream & CQ::CQstream::operator<<(const long long & l) { return (*this).append(l); }

CQstream & CQ::CQstream::operator<<(void(*control)(CQstream &)) { control(*this); return *this; }

void CQ::CQstream::FLUSH() { SEND(); }

inline CQ::CQstream::~CQstream() {}

inline CQ::logstream::logstream(std::string title, int Log_flag) : flag(Log_flag), title(title) {}

void CQ::logstream::SEND() { if (buf.size() <= 0)return; addLog(flag, title.c_str(), buf.c_str()); }

CQ::MsgSend::MsgSend(long long ID, msgtype Type) : ID(ID), subType(Type) {}

CQ::MsgSend::MsgSend(long long ID, int Type) : ID(ID), subType(Type) {}

void CQ::MsgSend::SEND() {
	if (buf.size() <= 0)return;
	switch (subType)
	{
	case msgtype::好友://好友
		sendPrivateMsg(ID, buf);
		break;
	case msgtype::群://群
		sendGroupMsg(ID, buf);
		break;
	case msgtype::讨论组://讨论组
		sendDiscussMsg(ID, buf);
		break;
	default:
		static Logger log("异常报告");
		log.Warning()
			<< "消息发送异常"
			<< ",类别:" << ID
			<< ",原文: " << buf
			<< send;
		break;
	}
}