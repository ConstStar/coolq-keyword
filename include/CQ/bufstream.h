#pragma once
#include <string>
namespace CQ {
	class CQstream {
	protected:
		std::string buf;
		bool raw = false;
		friend void RAW(CQstream & log);
		friend void CODE(CQstream & log);
	public:
		virtual	bool isRAW();
		virtual void clear();

		//字符串
		virtual	CQstream & append(const std::string & s);
		virtual	CQstream & operator <<(const std::string & s);
		//
		virtual	CQstream & append(const char * c);
		virtual	CQstream & operator <<(const char * c);

		//整数
		virtual	CQstream & append(const bool & i);
		virtual	CQstream & operator <<(const bool & i);
		virtual	CQstream & append(const int & i);
		virtual	CQstream & operator <<(const int & i);
		virtual	CQstream & append(const  size_t & i);
		virtual	CQstream & operator <<(const size_t & i);
		virtual	CQstream & append(const double & i);
		virtual	CQstream & operator <<(const double & i);
		virtual	CQstream & append(const long & i);
		virtual	CQstream & operator <<(const long & i);
		virtual	CQstream & append(const long long & l);
		virtual	CQstream & operator <<(const long long & l);

		//特殊控制符
		virtual	CQstream & operator <<(void(*control)(CQstream &));
		virtual	void SEND() = 0;
		virtual	void FLUSH();

		virtual ~CQstream();
	};
	//发送并清除缓冲区
	void send(CQstream & log);
	//只发送,保留缓冲区,下次发送时将发送重复内容
	void flush(CQstream & log);
	//换行
	void endl(CQstream & log);
	//输出原始字符而不是酷Q码
	void RAW(CQstream & log);
	//输出酷Q码
	void CODE(CQstream & log);
}