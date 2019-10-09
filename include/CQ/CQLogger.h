#pragma once
#include "cqdefine.h"
#include "CQconstant.h"
#include "bufstream.h"
#include <string>

#define DEBUGINFO "�ļ�:" << __FILE__ << ",����:" << __LINE__ << ",���:"
namespace CQ
{
	class logstream : public CQstream
	{
		int flag;
		std::string title;
	public:
		logstream(std::string title, int Log_flag);

		// ͨ�� CQstream �̳�
		virtual void SEND() override;
	};

	class Logger
	{
		std::string title;
	public:
		Logger(std::string title);
		void setTitle(std::string title);

		void Debug(std::string&msg)const;
		void Info(std::string&msg)const;
		void InfoSuccess(std::string&msg)const;
		void InfoRecv(std::string&msg)const;
		void InfoSend(std::string&msg)const;
		void Warning(std::string&msg)const;
		void Error(std::string&msg)const;
		void Fatal(std::string&msg)const;

		void Debug(const char* msg)const;
		void Info(const char* msg)const;
		void InfoSuccess(const char* msg)const;
		void InfoRecv(const char* msg)const;
		void InfoSend(const char* msg)const;
		void Warning(const char* msg)const;
		void Error(const char* msg)const;
		void Fatal(const char* msg)const;

		logstream Debug()const;
		logstream Info()const;
		logstream InfoSuccess()const;
		logstream InfoRecv()const;
		logstream InfoSend()const;
		logstream Warning()const;
		logstream Error()const;
		logstream Fatal()const;
	};
}
