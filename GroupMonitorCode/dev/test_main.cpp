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

#define WIN32_LEAN_AND_MEAN // 从 Windows 头中排除极少使用的资料
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
	cout << "解码后的原始数据: ";
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
		consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);//实例化句柄
	}
	inline void bfb(int len, int jd) {
		if (jd != 0 && len%jd != 0)return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(consolehwnd, &csbi);//获取当前坐标

		cout << "进度: " << len;

		SetConsoleCursorPosition(consolehwnd, csbi.dwCursorPosition);//回到历史坐标
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

		cout << "耗时: ";
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
			cout << "ms/每条)";
		}
		cout << endl;
	}
};

#include "sqlite/sqlite3.h"
#include "sqlite/sqlitetool.h"
void sqlite_test()
{
	cout << "sqlite测试项目里" << endl << endl;
	//插入次数
	auto file =
		"";
	// "sql.db";
	int lll = 100000;
	//初始化
	remove(file);

	sqlite3* db;
	sqlite3_stmt * stmt;

	auto create = "CREATE TABLE COMPANY(a TEXT,b TEXT)",
		insert = "INSERT INTO COMPANY (a,b) VALUES (123 , 456)",
		prepare_ = "INSERT INTO COMPANY VALUES (?,?)";
	auto
		BEGIN = U("BEGIN"),//事务开始
		COMMIT = U("COMMIT"),//事务结束
		CREATE = U(create),
		INSERT = U(insert),
		prepare = U(prepare_),
		t = U("123");
	auto len = strlen(t);
	int re;

	TimeTool k;

	//cpu执行周期
	cout << "执行  for (int i = 0; i < 10000; ++i);" << endl;
	k.st(); {
		for (int i = 0; i < 10000; ++i);
	}
	k.end();

	//数据库连接
	cout << endl
		<< "打开数据库" << endl
		<< "文件地址:" << file << endl
		<< "sqlite3_open(...); " << endl;
	k.st();
	{
		auto re = sqlite3_open(file, &db);
		if (re != SQLITE_OK) { cout << "数据库连接出错: " << G(sqlite3_errmsg(db)) << endl; return; }
	}
	k.end();

	//建表
	cout << endl
		<< "建表: sqlite3_exec(...);" << endl
		<< "T-SQL: CREATE TABLE COMPANY(a TEXT,b TEXT);" << endl;
	k.st();
	{
		re = sqlite3_exec(db, CREATE, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "数据库建立失败: " << G(sqlite3_errmsg(db)) << endl;  return; }
	}
	k.end();

	//方法一,每次都完整执行插入方法
	cout << endl
		<< "方法一,每次都完整执行插入方法" << endl
		<< "sqlite3_exec(...);(" << lll << "次)" << endl
		<< insert << endl;
	k.st();
	{
		for (int i = 0; i < lll; ++i) {
			k.bfb(i, lll / 1000);
			re = sqlite3_exec(db, INSERT, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "数据插入失败(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
		}
	}
	k.end(lll);

	//方法二,每次都完整执行插入方法(事务)
	cout << endl
		<< "方法二,每次都完整执行插入方法(事务)" << endl
		<< "sqlite3_exec(...);(" << lll << "次)" << endl
		<< insert << endl;
	k.st();
	{
		re = sqlite3_exec(db, BEGIN, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "事务打开失败!" << endl; }
		else {
			for (int i = 0; i < lll; ++i) {
				k.bfb(i, lll / 1000);
				re = sqlite3_exec(db, INSERT, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "数据插入失败(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
			}
			sqlite3_exec(db, COMMIT, nullptr, 0, nullptr);
		}
	}
	k.end(lll);

	re = sqlite3_prepare_v2(db, prepare, strlen(prepare), &stmt, nullptr); if (re != SQLITE_OK) { cout << "语句准备失败: " << G(sqlite3_errmsg(db)) << endl; }
	re = sqlite3_bind_text(stmt, 1, t, len, nullptr); if (re != SQLITE_OK) { cout << "数据准备失败1: " << G(sqlite3_errmsg(db)) << endl; }
	re = sqlite3_bind_text(stmt, 2, t, len, nullptr); if (re != SQLITE_OK) { cout << "数据准备失败2: " << G(sqlite3_errmsg(db)) << endl; }
	//方法三,执行准备
	cout << endl
		<< "方法三,执行准备" << endl
		<< "sqlite3_prepare_v2;(" << lll << "次)" << endl
		<< prepare_ << endl;
	k.st();
	{
		for (int i = 0; i < lll; ++i) {
			k.bfb(i, lll / 1000);
			re = sqlite3_step(stmt); if (re != SQLITE_DONE) { cout << "数据插入失败(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
		}
	}
	k.end(lll);

	//方法四,执行准备(事务)
	cout << endl
		<< "方法四,执行准备(事务)" << endl
		<< "sqlite3_prepare_v2;(" << lll << "次)" << endl
		<< prepare_ << endl;
	k.st();
	{
		re = sqlite3_exec(db, BEGIN, nullptr, 0, nullptr); if (re != SQLITE_OK) { cout << "事务打开失败!" << endl; }
		else {
			for (int i = 0; i < lll; ++i) {
				k.bfb(i, lll / 1000);
				re = sqlite3_step(stmt); if (re != SQLITE_DONE) { cout << "数据插入失败(" << i << "): " << G(sqlite3_errmsg(db)) << endl; break; }
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
		if (Speak_to_wav_file("D:\\~sys\\Desktop\\1.wav", "你们想知道什么"))
		{
			cout << "成功生成" << endl;
		}
		else {
			cout << "生成失败" << endl;
		}
		k.end();
	}
	else {
		cout << "初始化失败" << endl;
	}
}
void main() {
	//sqlite_test();
	ini_test();
	//unpack_test();
	//Speak_test()

	cin.get();
}