#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <iostream>

#include "..\..\inculde\CQAPI.h"

using namespace std;

typedef int(__stdcall*fun1)();
typedef int(__stdcall*fun2)(int subType, int msgId, long long fromQQ, const char* msg, int font);

fun1 Enable; fun2 PrivateMsg1;
decltype(LoadLibrary("")) dll;

void ����DLL() {
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);

	string s(pBuf);
	s += "\\python36.dll";
	string c(pBuf);
	c += "\\CQP.dll";

	if (!LoadLibrary("python36.dll")) { cout << "python36����ʧ��:" << GetLastError() << endl; return; }
	if (!LoadLibrary("CQP.dll")) { cout << "CQP����ʧ��:" << GetLastError() << endl; return; }
	if (!(dll = ::LoadLibrary("cn.mikupy2001.PYruntime.dll"))) { cout << "dll����ʧ��:" << GetLastError() << endl; return; }
}

void ����SDK() {
	Enable = (fun1)GetProcAddress(dll, "Enable");
	PrivateMsg1 = (fun2)GetProcAddress(dll, "PrivateMsg1");

	cout << "[Run]" << "Enable" << endl;
	Enable();
}
void ����SDK() {
	cout << "[Run]" << "PrivateMsg1" << endl;
	PrivateMsg1(1, 2, 3, "123", 0);

	cout << "[Run]" << "�������" << endl;
}
void ����() {

}
int main() {
	����DLL();
	����SDK();
	//����();

	����SDK();

	cin.get();
	return 0;
}