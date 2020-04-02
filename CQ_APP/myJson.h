#pragma once

#include <vector>
#include <map>
#include <string>

#include <json/json.h>
#include <iostream>


using namespace std;




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



struct WKEYWORD
{
	WKEYWORD(string& str) :keyWord(str), wkeyWrod(OperateStr::string2wstring(str))
	{}

	WKEYWORD(const char* str) :keyWord(str), wkeyWrod(OperateStr::string2wstring(string(str)))
	{}

	string keyWord;
	wstring wkeyWrod;

};

//���е������õ���Ŀ
struct ConfAlone
{

	vector<WKEYWORD> keyWord;		//��ͨ�ؼ���		
	vector<WKEYWORD> keyWordRegex;	//������ʽ�ؼ���
	vector<WKEYWORD> keyWordWhite;	//�������ؼ���
	vector<long long> groupList;	//Ⱥ�б�
	vector<long long> QQList;		//QQ��������
	vector<long long> relayGroupList;//ת��Ⱥ��Ϣ

	int dealType;						//������
	int banTimeLen;					//����ʱ��
	int QQListType;					//������������  ������||�������
	bool groupWarn;					//�������Ƿ�Ⱥ�ھ���
	bool streng;					//ǿ�����
	bool revoke;					//��Ϣ����

	string relayGroupWord;			//���͵�Ⱥ ��Ϣ��ʽ
	int relayGroupMsg_frontLine;	//���͵�Ⱥ ��Ϣǰ����ɾ��
	int relayGroupMsg_afterLine;	//���͵�Ⱥ ��Ϣ����ɾ��

	string keyWordGroupWarn;		//�����ؼ���Ⱥ�ھ���
};

////����
//struct Conf
//{
//	
//};


class MyJson
{
private:

	//д��json
	bool write_json(string path, Json::Value& root);

	//��ȡjson
	bool read_json(string path, Json::Value& root);

	//��ȡkey�б�
	vector<string> getKeyList(vector<string> json_path);

	//����json�е�����
	bool readJson_vector(vector<string> json_path, vector<string>& value);

	//����json�е�����
	bool readJson_vector(vector<string> json_path, vector<long long>& value);

	//����json�е�����
	bool writeJson_vector(vector<string> json_path, vector<string>& value);

	//����json�е�����
	bool writeJson_vector(vector<string> json_path, vector<long long>& value);

	//��ȡjson
	template <typename T>
	T get(vector<string> json_path, T def = T());
	

	//����json
	template <typename T>
	void put(vector<string> json_path, T value);


public:


	//��keyword������ŵ�json������
	bool keyword2json();

	//��json������ŵ�keyword������
	bool json2keyword();

	//��REkeyword������ŵ�json������
	bool REkeyword2json();

	//��json������ŵ�REkeyword������
	bool json2REkeyword();

	//��keywordWhite������ŵ�json������
	bool keywordWhite2json();

	//��json������ŵ�keywordWhite������
	bool json2keywordWhite();

	//�� ���Ⱥ���� ��ŵ�json�е�
	bool GroupList2json();

	//�� json ��ŵ� ���Ⱥ���� ��
	bool json2GroupList();

	//�� QQ������/������� ��ŵ�json�е�
	bool qqlist2json();

	//�� json ��ŵ�QQ������/���������
	bool json2qqlist();

	//�� ����Ⱥ���� ��ŵ�json�е�
	bool sendGroupList2json();

	//�� json ��ŵ� ����Ⱥ���� ��
	bool json2sendGroupList();

	//�� root ��ŵ�json�е�
	bool admin2json();

	//�� json ��ŵ�root��
	bool json2admin();

	//��ȡ��Ҫ����json���ڴ���
	bool json2main();

	//���ڴ���Ҫ�����з��õ�json
	bool main2json();

	//��ȡȺ����json���ڴ���
	bool json2groupConf();

	//��Ⱥ�����з��õ�json
	bool groupConf2json();



	//�����еĴ�ŵ�json�е�
	void all2json();

	//���е�json��ȡ���ڴ���
	void json2all();

	//��jsonд���ļ���
	void json2file();

	//�ļ���json��ȡ����
	void file2json();

public:

	//����
	map<int, ConfAlone> alone;		//��������
	vector<long long> admin;		//����QQ
	string prefix;					//ָ����Ϣǰ׺
	bool msgRelay;					//��Ϣת��

	//Json
	Json::Value conf_json;

};



class OperateLine
{
public:
	//��ȡ �ָ�ÿ�е��ַ��� json
	static void line_get_str(string str, vector<string>& value);

	//��ȡ �ָ�ÿ�е�uint json
	static void line_get_ll(string str, vector<long long>& value);
};
