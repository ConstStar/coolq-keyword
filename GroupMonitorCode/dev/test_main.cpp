#ifdef _DEBUG
#pragma comment(lib,"SDK_Debug.lib")
#else
#pragma comment(lib,"SDK_Release.lib")
#endif
#include "Unpack.h"
#include "CQTools.h"
#include "CQEVE_Status.h"
#include <CQAPI_EX.h>

#include <iostream>
#include <atlbase.h>
#include <thread>

#define WIN32_LEAN_AND_MEAN // �� Windows ͷ���ų�����ʹ�õ�����
#include <Windows.h>

using namespace std;
using namespace CQ;

auto getG(long long G) {
	for (auto one : getGroupMemberList(G))
		if (one.permissions == 3)
			return one;
}

void unpack_test()
{
	cout << "unpack_test" << endl;
	auto msg1 = "AAAAAAAPQkMAA0JZMgAoqB7sRA+yXhCpbcJQOu2dcQyaZu7ZbH79b74nQDlKY2p7TkWXXahTeQ==";
	auto msg2 = base64_decode(msg1);
	cout << "������ԭʼ����: ";
	dump((void*)msg2.data(), msg2.size());
	Unpack p(msg2);
	cout << p.getLong() << endl;
	cout << p.getstring().c_str() << endl;
}

void ini_test()
{
}

class TimeTool {
	LARGE_INTEGER LARGE_st, LARGE_end, nFreq;
	decltype(GetCycleCount()) cputime_st, cputime_end;
	HANDLE consolehwnd;
public:
	TimeTool() {
		QueryPerformanceFrequency(&nFreq);
		consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);//ʵ�������
	}
	inline void bfb(int len, int jd) {
		if (jd != 0 && len%jd != 0)return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(consolehwnd, &csbi);//��ȡ��ǰ����

		cout << "����: " << len;

		SetConsoleCursorPosition(consolehwnd, csbi.dwCursorPosition);//�ص���ʷ����
	}
	inline void st() {
		QueryPerformanceCounter(&LARGE_st);
		cputime_st = GetCycleCount();
	}
	inline void end(int len = 0) {
		cputime_end = GetCycleCount();
		QueryPerformanceCounter(&LARGE_end);
		auto dt = (LARGE_end.QuadPart - LARGE_st.QuadPart) / (double)nFreq.QuadPart;
		dt *= 1000;//ms

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(consolehwnd, &csbi);
		WORD colorOld = csbi.wAttributes;

		cout << "��ʱ: ";
		SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << cputime_end - cputime_st;
		SetConsoleTextAttribute(consolehwnd, colorOld);
		cout << "(";
		SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << dt;
		SetConsoleTextAttribute(consolehwnd, colorOld);
		cout << "ms)";

		if (len != 0) {
			cout << "(";
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << dt / len;
			SetConsoleTextAttribute(consolehwnd, colorOld);
			cout << "ms/ÿ��)";
		}
		cout << endl;
	}
};

#include "sqlite/sqlite3.h"
#include "sqlite/sqlitetool.h"
void sqlite_test()
{
	cout << "sqlite������Ŀ��" << endl << endl;
	//�������
	auto file =
		"";
	// "sql.db";
	int lll = 100000;
	//��ʼ��
	remove(file);

	sqlite3* db;
	sqlite3_stmt * stmt;

	auto create = "CREATE TABLE COMPANY(a TEXT,b TEXT)",
		insert = "INSERT INTO COMPANY (a,b) VALUES (123 , 456)",
		prepare_ = "INSERT INTO COMPANY VALUES (?,?)";
	auto
		BEGIN = U("BEGIN"),//����ʼ
		COMMIT = U("COMMIT"),//�������
		CREATE = U(create),
		INSERT = U(insert),
		prepare = U(prepare_),
		t = U("123");
	auto len = strlen(t);
	int re;

	TimeTool k;

	//cpuִ������
	cout << "ִ��  for (int i = 0; i < 10000; ++i);" << endl;
	k.st(); {
		for (int i = 0; i < 10000; ++i);
	}
	k.end();

	//���ݿ�����
	cout << endl
		<< "�����ݿ�" << endl
		<< "�ļ���ַ:" << file << endl
		<< "sqlite3_open(...); " << endl;
	k.st();
	{
		auto re = sqlite3_open(file, &db);
		if (re != SQLITE_OK) { cout << "���ݿ����ӳ���: " << G(sqlite3_errmsg(db)) << endl; return; }
	}
	k.end();

	//����
	cout << endl
		<< "����: sqlite3_exec(...);" << endl
		<< "T-SQL: CREATE TABLE COMPANY(a TEXT,b TEXT);" << endl;
	k.st();
	{
		re = sqlite3_exec(db, CREATE, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "���ݿ⽨��ʧ��: " << G(sqlite3_errmsg(db)) << endl;  return; }
	}
	k.end();

	//����һ,ÿ�ζ�����ִ�в��뷽��
	cout << endl
		<< "����һ,ÿ�ζ�����ִ�в��뷽��" << endl
		<< "sqlite3_exec(...);(" << lll << "��)" << endl
		<< insert << endl;
	k.st();
	{
		for (int i = 0; i < lll; ++i) {
			k.bfb(i, lll / 1000);
			re = sqlite3_exec(db, INSERT, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "���ݲ���ʧ��(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
		}
	}
	k.end(lll);

	//������,ÿ�ζ�����ִ�в��뷽��(����)
	cout << endl
		<< "������,ÿ�ζ�����ִ�в��뷽��(����)" << endl
		<< "sqlite3_exec(...);(" << lll << "��)" << endl
		<< insert << endl;
	k.st();
	{
		re = sqlite3_exec(db, BEGIN, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "�����ʧ��!" << endl; }
		else {
			for (int i = 0; i < lll; ++i) {
				k.bfb(i, lll / 1000);
				re = sqlite3_exec(db, INSERT, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "���ݲ���ʧ��(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
			}
			sqlite3_exec(db, COMMIT, nullptr, 0, nullptr);
		}
	}
	k.end(lll);

	re = sqlite3_prepare_v2(db, prepare, strlen(prepare), &stmt, nullptr); if (re != SQLITE_OK) { cout << "���׼��ʧ��: " << G(sqlite3_errmsg(db)) << endl; }
	re = sqlite3_bind_text(stmt, 1, t, len, nullptr); if (re != SQLITE_OK) { cout << "����׼��ʧ��1: " << G(sqlite3_errmsg(db)) << endl; }
	re = sqlite3_bind_text(stmt, 2, t, len, nullptr); if (re != SQLITE_OK) { cout << "����׼��ʧ��2: " << G(sqlite3_errmsg(db)) << endl; }
	//������,ִ��׼��
	cout << endl
		<< "������,ִ��׼��" << endl
		<< "sqlite3_prepare_v2;(" << lll << "��)" << endl
		<< prepare_ << endl;
	k.st();
	{
		for (int i = 0; i < lll; ++i) {
			k.bfb(i, lll / 1000);
			re = sqlite3_step(stmt); if (re != SQLITE_DONE) { cout << "���ݲ���ʧ��(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
		}
	}
	k.end(lll);

	//������,ִ��׼��(����)
	cout << endl
		<< "������,ִ��׼��(����)" << endl
		<< "sqlite3_prepare_v2;(" << lll << "��)" << endl
		<< prepare_ << endl;
	k.st();
	{
		re = sqlite3_exec(db, BEGIN, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "�����ʧ��!" << endl; }
		else {
			for (int i = 0; i < lll; ++i) {
				k.bfb(i, lll / 1000);
				re = sqlite3_step(stmt); if (re != SQLITE_DONE) { cout << "���ݲ���ʧ��(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
			}
			sqlite3_exec(db, COMMIT, nullptr, 0, nullptr);
		}
	}
	k.end(lll);
	sqlite3_finalize(stmt);

	sqlite3_close(db);
}

#include <winSpeak.h>
void Speak_test() {
	TimeTool k;
	k.st();
	auto hr = Speak_init();
	k.end();
	if (hr)
	{
		k.st();
		if (Speak_to_wav_file("D:\\~sys\\Desktop\\1.wav", "������֪��ʲô"))
		{
			cout << "�ɹ�����" << endl;
		}
		else {
			cout << "����ʧ��" << endl;
		}
		k.end();
	}
	else {
		cout << "��ʼ��ʧ��" << endl;
	}
}
void main() {
	//sqlite_test();
	ini_test();
	//unpack_test();
	//Speak_test()

	cin.get();
}