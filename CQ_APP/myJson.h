#pragma once

#include <vector>
#include <map>
#include <string>

#include <json/json.h>
#include <iostream>
#include <functional> 

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
	bool use;						//����
	int priority;					//���ȼ�
	string name;					//��������

	vector<WKEYWORD> keyWord;		//��ͨ�ؼ���		
	vector<WKEYWORD> keyWordRegex;	//������ʽ�ؼ���
	vector<WKEYWORD> keyWordWhite;	//�������ؼ���
	vector<long long> groupList;	//Ⱥ�б�
	vector<long long> QQList;		//QQ��������
	vector<long long> relayGroupList;//ת��Ⱥ��Ϣ

	int dealType;						//������
	int banTimeLen;					//����ʱ��
	int QQListType;					//������������  ������||�������
	bool keyWordGroupWarn;			//�������Ƿ�Ⱥ�ھ���
	bool streng;					//ǿ�����
	bool deleteMsg;					//��Ϣ����

	string relayGroupWord;			//���͵�Ⱥ ��Ϣ��ʽ
	int relayGroupMsg_trimFront;	//���͵�Ⱥ ��Ϣǰ����ɾ��
	int relayGroupMsg_trimBack;		//���͵�Ⱥ ��Ϣ����ɾ��

	string keyWordGroupWarnWord;		//�����ؼ���Ⱥ�ھ���
};

////����
//struct Conf
//{
//	
//};

//map����
struct CmpByKeySize {
	bool operator()(const int& k1, const int& k2)const {
		return k1 < k2;
	}
};

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

	//�� root ��ŵ�json�е�
	bool admin2json();

	//�� json ��ŵ�root��
	bool json2admin();

	//��ȡ��Ҫ����json���ڴ���
	bool json2mainSwitch();

	//���ڴ���Ҫ�����з��õ�json
	bool mainSwitch2json();

	//��ȡ�������õ��ڴ�
	bool json2alone();

	//���������÷���json
	bool alone2json();

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
	map<int, ConfAlone, CmpByKeySize> alone;		//��������
	vector<long long> admin;		//����QQ
	string prefix;					//ָ����Ϣǰ׺
	bool relayPrivateMsg;			//��Ϣת��

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
