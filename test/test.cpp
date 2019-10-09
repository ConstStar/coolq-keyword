//// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
////
#include "pch.h"
#include <map>
#include <string>
#include <Windows.h>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/dll.hpp>

using namespace std;
using namespace boost::dll;

using namespace boost::property_tree;

string AppFile = "../";

//map<long long, std::string> get(*GroupList)()
//{
//	map<long long, std::string> a;
//	a[1164442003] = "xxxx";
//	a[1164442004] = "xxx";
//	a[1164442005] = "xxx";
//	a[1164442006] = "xxx";
//	return a;
//}

void CQ_getGroupList_charp(string& str)
{
	ptree tt;
	tt.put<string>("453370241", "2哈1");
	tt.put<string>("453374441", "121昂");
	tt.put<string>("533066241", "1哈希1");
	tt.put<string>("437055241", "黑12");


	std::stringstream s2;
	write_json(s2, tt);
	str = s2.str();
}

//int main()
//try
//{
//	string str;
//	CQ_getGroupList_charp(str);
//
//	boost::filesystem::path pathDLL = L"GroupMonitorWin.dll";
//	std::function<void(const char* Path, const char* fun)> funcExt = boost::dll::import<void(const char* Path, const char* fun)>(pathDLL, "winfun");
//
//	funcExt(AppFile.c_str(), str.c_str());
//
//
//	return 0;
//}
//catch (exception& e)
//{
//	cout << e.what() << endl;
//}

int main()
try
{
	cout << "测试" << endl;

	string str;
	CQ_getGroupList_charp(str);
	//CQ_get(*GroupList)_fun(fun);


	/*DllFun a;
	a.get(*GroupList) = get(*GroupList);*/
	typedef void(*Pwinfun)(const char* Path, const char* fun);
	HINSTANCE hDLL;
	Pwinfun win;
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //加载动态链接库MyDll.dll文件；
	if (hDLL == NULL) {
		MessageBoxA(NULL, "界面打开失败，可能是你没有复制界面dll\n有问题欢迎到QQ群：839067703 求助", "错误", NULL);
		//logger.Info("界面打开失败");
		return -1;
	}
	win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	win(AppFile.c_str(), str.c_str());
	FreeLibrary(hDLL);

	Sleep(5000);
	return 0;
}
catch (exception &e)
{
	cout << "exception " << e.what() << endl;
}