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


	//��keyword������ŵ�json������
	bool keyWord2json();

	//��json������ŵ�keyword������
	bool json2keyWord();

	//��REkeyword������ŵ�json������
	bool keyWordRegex2json();

	//��json������ŵ�REkeyword������
	bool json2keyWordRegex();

	//��keywordWhite������ŵ�json������
	bool keyWordWhite2json();

	//��json������ŵ�keywordWhite������
	bool json2keyWordWhite();

	//�� ���Ⱥ���� ��ŵ�json�е�
	bool groupList2json();

	//�� json ��ŵ� ���Ⱥ���� ��
	bool json2groupList();

	//�� QQ������/������� ��ŵ�json�е�
	bool QQlist2json();

	//�� json ��ŵ�QQ������/���������
	bool json2QQlist();

	//�� ת��Ⱥ���� ��ŵ�json�е�
	bool relayGroupList2json();

	//�� json ��ŵ� ת��Ⱥ���� ��
	bool json2relayGroupList();

	//�� root ��ŵ�json�е�
	bool admin2json();

	//�� json ��ŵ�root��
	bool json2admin();

	//��ȡ��Ҫ����json���ڴ���
	bool json2mainSwitch();

	//���ڴ���Ҫ�����з��õ�json
	bool mainSwitch2json();

	//��ȡȺ����json���ڴ���
	bool json2aloneSwitch();

	//��Ⱥ�����з��õ�json
	bool aloneSwitch2json();


	bool json2alone()
	{
		try
		{
			auto keyList = getKeyList({ "alone" });

			for (auto temp_key : keyList)
			{
				auto& aloneTemp = alone[atoi(temp_key.c_str())];

				//��ͨ�ؼ���
				vector<string> temp_keyword;
				auto& keyWord = aloneTemp.keyWord;
				readJson_vector({ "alone",temp_key,"keyWord" }, temp_keyword);
				keyWord.clear();
				for (auto temp_keyWord : temp_keyword)
				{
					keyWord.push_back(temp_keyWord);
				}

				//������ʽ�ؼ���
				vector<string> temp_keywordRegex;
				auto& keyWordRegex = aloneTemp.keyWordRegex;
				readJson_vector({ "alone",temp_key,"keyWordRegex" }, temp_keywordRegex);

				keyWordRegex.clear();
				for (auto temp_keyWordRegex : temp_keywordRegex)
				{
					keyWordRegex.push_back(temp_keyWordRegex);
				}

				//�������ؼ���
				vector<string> temp_keywordWhite;
				auto& keyWordWhite = aloneTemp.keyWordWhite;
				readJson_vector({ "alone",temp_key,"keyWordWhite" }, temp_keywordWhite);
				keyWordWhite.clear();
				for (auto temp_keyWordRegex : temp_keywordWhite)
				{
					keyWordWhite.push_back(temp_keyWordRegex);
				}

				readJson_vector({ "alone",temp_key,"groupList" }, aloneTemp.groupList);
				readJson_vector({ "alone",temp_key,"QQList" }, aloneTemp.QQList);
				readJson_vector({ "alone",temp_key,"relayGroupList" }, aloneTemp.relayGroupList);


				aloneTemp.keyWordGroupWarn = get<bool>({ "alone",temp_key, "groupWarn" }, false);
				aloneTemp.deleteMsg = get<bool>({ "alone",temp_key, "deleteMsg" }, false);
				aloneTemp.streng = get<bool>({ "alone",temp_key, "streng" }, false);

				aloneTemp.relayGroupMsg_trimFront = get<int>({ "alone",temp_key, "relayGroupMsg_trimFront" }, 0);
				aloneTemp.relayGroupMsg_trimBack = get<int>({ "alone",temp_key, "relayGroupMsg_trimBack" }, 0);
				aloneTemp.QQListType = get<int>({ "alone",temp_key, "QQListType" }, 0);
				aloneTemp.dealType = get<int>({ "alone",temp_key, "dealType" }, 0);
				aloneTemp.banTimeLen = get<int>({ "alone",temp_key, "banTimeLen" }, 0);
				aloneTemp.priority = get<int>({ "alone",temp_key, "priority" }, 0);

				aloneTemp.name = get<string>({ "alone",temp_key, "name" }, "");
				aloneTemp.relayGroupWord = get<string>({ "alone",temp_key, "relayGroupWord" }, "");
				aloneTemp.keyWordGroupWarnWord = get<string>({ "alone",temp_key, "keyWordGroupWarn" }, "");
			}
		}
		catch (exception & e)
		{
			cout << e.what() << endl;
			return false;
		}

		return true;

	}


	bool alone2json()
	{

		try
		{
			int index = 1;
			for (auto& temp : alone)
			{

				string temp_key = "0";
				auto& aloneTemp = temp.second;

				//��������
				if (temp.first != 0)
					temp_key = to_string(index++);

				//��ͨ�ؼ���
				vector<string> temp_keyword;
				for (auto temp_word : aloneTemp.keyWord)
				{
					temp_keyword.push_back(temp_word.keyWord);
				}

				writeJson_vector({ "alone", temp_key ,"keyWord" }, temp_keyword);

				//������ʽ�ؼ���
				vector<string> temp_keyWordRegex;
				for (auto temp_wordRegex : aloneTemp.keyWordRegex)
				{
					temp_keyWordRegex.push_back(temp_wordRegex.keyWord);
				}

				writeJson_vector({ "alone", temp_key ,"keyWordRegex" }, temp_keyWordRegex);

				//�������ؼ���
				vector<string> temp_keyWordWhite;
				for (auto temp_wordWhite : aloneTemp.keyWordWhite)
				{
					temp_keyWordWhite.push_back(temp_wordWhite.keyWord);
				}
				writeJson_vector({ "alone", temp_key ,"keyWordWhite" }, temp_keyWordWhite);


				writeJson_vector({ "alone",temp_key,"groupList" }, aloneTemp.groupList);
				writeJson_vector({ "alone",temp_key,"QQList" }, aloneTemp.QQList);
				writeJson_vector({ "alone",temp_key,"relayGroupList" }, aloneTemp.relayGroupList);


				put<bool>({ "alone",temp_key,"groupWarn" }, aloneTemp.keyWordGroupWarn);
				put<bool>({ "alone",temp_key,"deleteMsg" }, aloneTemp.deleteMsg);
				put<bool>({ "alone",temp_key,"streng" }, aloneTemp.streng);

				put<int>({ "alone",temp_key,"relayGroupMsg_trimFront" }, aloneTemp.relayGroupMsg_trimFront);
				put<int>({ "alone",temp_key,"relayGroupMsg_trimBack" }, aloneTemp.relayGroupMsg_trimBack);
				put<int>({ "alone",temp_key,"QQListType" }, aloneTemp.QQListType);
				put<int>({ "alone",temp_key,"dealType" }, aloneTemp.dealType);
				put<int>({ "alone",temp_key,"banTimeLen" }, aloneTemp.banTimeLen);
				put<int>({ "alone",temp_key,"priority" }, aloneTemp.priority);

				put<string>({ "alone",temp_key,"name" }, aloneTemp.name);
				put<string>({ "alone",temp_key,"relayGroupWord" }, aloneTemp.relayGroupWord);
				put<string>({ "alone",temp_key,"keyWordGroupWarn" }, aloneTemp.keyWordGroupWarnWord);
			}
		}
		catch (exception & e)
		{
			cout << e.what() << endl;
			return false;
		}

		return true;

	}








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
