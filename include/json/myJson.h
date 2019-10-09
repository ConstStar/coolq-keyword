#pragma once

//#define DEBUG

#ifdef MFC
#include <afx.h>
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <map>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


using namespace std;
using namespace boost::property_tree;



//�ַ�������
class OperateStr
{
public:
	//
	static string& replace_all(string& str, const  string& old_value, const  string& new_value);

	//�ַ���ȫ���滻
	static string replace_all_distinct(string& str, string old_value, string new_value);

	//��stringת��wstring
	static wstring string2wstring(string str);

	//��wstringת����string  
	static string wstring2string(wstring wstr);
};

#ifndef MFC

struct WKEYWORD
{
	WKEYWORD(string& str) :keyWord(str), wkeyWrod(OperateStr::string2wstring(str))
	{}

	WKEYWORD(const char* str) :keyWord(str), wkeyWrod(OperateStr::string2wstring(string(str)))
	{}

	string keyWord;
	wstring wkeyWrod;

};

#endif // !MFC

struct CONF
{
	int way;//������
	int wayTime;//����ʱ��
	int typeQQList;//��������  ������/�������
	bool GroupWarn;//�������Ƿ�Ⱥ�ھ���
	bool Streng;//ǿ�����
	bool Revoke;//��Ϣ����

	string sendGroupMsg_word;//���͵�Ⱥ ��Ϣ��ʽ
	int sendGroupMsg_frontLine;//���͵�Ⱥ ��Ϣǰ����ɾ��
	int sendGroupMsg_afterLine;//���͵�Ⱥ ��Ϣ����ɾ��

	string WordKeyWarn;//�����ؼ���Ⱥ����������
};

extern wptree conf_json;
extern string AppFile;

#ifdef MFC

extern map<long long, vector<string>> g_keyword;
#else
extern map<long long, vector<WKEYWORD>> g_keyword;

#endif // MFC

extern map<long long, vector<string>> g_REKeyWord;
extern map<long long, vector<long long>> g_qqlist;
extern map<long long, vector<long long>> g_sendGroupList;
extern vector<long long> g_GroupList;
extern vector<long long> g_AlGroupList;
extern map<long long, vector<string>> g_KeyWordWhite;
extern vector<long long> g_root;
extern map<long long, CONF> g_conf;
extern bool g_MsgRelay;//��Ϣת��
extern string g_prefix;//ָ����Ϣǰ׺

#ifdef MFC

extern map<long long, string> WinGroupList;

#endif // MFC


class OperateFile
{

public:

	//����json�е�����
	static bool json_json2vector_str(const wchar_t* json_path, vector<string>& value);

	static bool json_json2vector_longlong(const wchar_t* json_path, vector<long long>& value);


	//����json�е�����
	static bool json_vector2json_str(const wchar_t* json_path, const wchar_t* json_after, vector<string>& value);


	static bool json_vector2json_longlong(const wchar_t* json_path, const wchar_t* json_after, vector<long long>& value);


	//Ⱥ��������map ��ŵ�json

	static bool json_map2json_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value);


	static bool json_map2json_str(const wchar_t* json_after, map<long long, vector<string>>& value);


	//Ⱥ��������json ��ȡmap
	static bool json_json2map_str(const wchar_t* json_after, map<long long, vector<string>>& value);


	static bool json_json2map_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value);



	//��keyword������ŵ�json������
	static bool keyword2json();

	//��json������ŵ�keyword������
	static bool json2keyword();

	//��REkeyword������ŵ�json������
	static bool REkeyword2json();

	//��json������ŵ�REkeyword������
	static bool json2REkeyword();

	//��keywordWhite������ŵ�json������
	static bool keywordWhite2json();

	//��json������ŵ�keywordWhite������
	static bool json2keywordWhite();

	//�� ���Ⱥ���� ��ŵ�json�е�
	static bool GroupList2json();

	//�� json ��ŵ� ���Ⱥ���� ��
	static bool json2GroupList();

	//�� �������Ⱥ���� ��ŵ�json�е�
	static bool AlGroupList2json();

	//�� json ��ŵ� �������Ⱥ���� ��
	static bool json2AlGroupList();

	//�� QQ������/������� ��ŵ�json�е�
	static bool qqlist2json();

	//�� json ��ŵ�QQ������/���������
	static bool json2qqlist();

	//�� ����Ⱥ���� ��ŵ�json�е�
	static bool sendGroupList2json();

	//�� json ��ŵ� ����Ⱥ���� ��
	static bool json2sendGroupList();

	//�� root ��ŵ�json�е�
	static bool root2json();

	//�� json ��ŵ�root��
	static bool json2root();

	//��ȡ��Ҫ����json���ڴ���
	static bool json2main();

	//���ڴ���Ҫ�����з��õ�json
	static bool main2json();

	//��ȡȺ����json���ڴ���
	static bool json2groupConf();

	//��Ⱥ�����з��õ�json
	static bool groupConf2json();



	//�����еĴ�ŵ�json�е�
	static void all2json();

	//���е�json��ȡ���ڴ���
	static void json2all();

	//��jsonд���ļ���
	static void json2file();

	//�ļ���json��ȡ����
	static void file2json();

#ifdef MFC

	static void json2WinGroupList(const char* GroupList_str);


#endif // MFC

};

class OperateLine
{
public:
#ifdef MFC
	//���� �ָ�ÿ�е��ַ��� ����ӵ�json
	static void line_put_str(CString& str, vector<string>& value);

	//���� �ָ�ÿ�е�uint ����ӵ�json
	static void line_put_ll(CString& str, vector<long long>& value);

	//��ȡ �ָ�ÿ�е��ַ��� json
	static void line_get_str(CString& str, vector<string>& value);

	//��ȡ �ָ�ÿ�е�uint json
	static void line_get_ll(CString& str, vector<long long>& value);
#else

	//��ȡ �ָ�ÿ�е��ַ��� json
	static void line_get_str(string str, vector<string>& value);

	//��ȡ �ָ�ÿ�е�uint json
	static void line_get_ll(string str, vector<long long>& value);


#endif // MFC
};
