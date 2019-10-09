#include "py_ת��.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

using namespace std;

//gbkתUTF-8
//�ش���char*��������
shared_ptr<char*> U(const char* strGbk)//�����strGbk��GBK����
{
	char* strUtf8;
	//delete[]strUtf8;

	//gbkתunicode
	auto len = MultiByteToWideChar(CP_ACP, 0, strGbk, -1, NULL, 0);
	auto *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strGbk, -1, strUnicode, len);

	//unicodeתUTF-8
	len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	strUtf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

	//��ʱ��strUtf8��UTF-8����
	delete[]strUnicode;
	strUnicode = NULL;
	return make_shared<char*>(strUtf8);
}

//UTF-8תgbk
//�ش���char*��������
shared_ptr<char*> G(const char* strUtf8)//�����strUtf8��UTF-8����
{
	char *strGbk;
	//delete[]strGbk;
	//UTF-8תunicode
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
	wchar_t * strUnicode = new wchar_t[len];//len = 2
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, strUnicode, len);

	//unicodeתgbk
	len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	strGbk = new char[len];//len=3 ����Ϊ2������char*�����Զ�������\0
	memset(strGbk, 0, len);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strGbk, len, NULL, NULL);

	//��ʱ��strTemp��GBK����
	delete[]strUnicode;
	strUnicode = NULL;
	return make_shared<char*>(strGbk);
}