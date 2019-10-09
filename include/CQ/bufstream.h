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

		//�ַ���
		virtual	CQstream & append(const std::string & s);
		virtual	CQstream & operator <<(const std::string & s);
		//
		virtual	CQstream & append(const char * c);
		virtual	CQstream & operator <<(const char * c);

		//����
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

		//������Ʒ�
		virtual	CQstream & operator <<(void(*control)(CQstream &));
		virtual	void SEND() = 0;
		virtual	void FLUSH();

		virtual ~CQstream();
	};
	//���Ͳ����������
	void send(CQstream & log);
	//ֻ����,����������,�´η���ʱ�������ظ�����
	void flush(CQstream & log);
	//����
	void endl(CQstream & log);
	//���ԭʼ�ַ������ǿ�Q��
	void RAW(CQstream & log);
	//�����Q��
	void CODE(CQstream & log);
}