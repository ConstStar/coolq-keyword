#pragma once

#include <vector>
#include <map>
#include <string>

#include <json/json.h>
#include <iostream>
#include <functional> 

using namespace std;




//字符串处理
class OperateStr
{
public:
	//
	static string& replace_all(string& str, const  string& old_value, const  string& new_value);

	//字符串全部替换
	static string replace_all_distinct(string& str, string old_value, string new_value);

	//将string转成wstring
	static wstring string2wstring(string str);

	//将wstring转换成string  
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

//具有单独设置的项目
struct ConfAlone
{
	int priority;					//优先级
	string name;					//设置名称

	vector<WKEYWORD> keyWord;		//普通关键词		
	vector<WKEYWORD> keyWordRegex;	//正则表达式关键词
	vector<WKEYWORD> keyWordWhite;	//白名单关键词
	vector<long long> groupList;	//群列表
	vector<long long> QQList;		//QQ特殊名单
	vector<long long> relayGroupList;//转发群消息

	int dealType;						//处理方法
	int banTimeLen;					//禁言时长
	int QQListType;					//特殊名单类型  白名单||监控名单
	bool keyWordGroupWarn;			//触发后是否群内警告
	bool streng;					//强力检测
	bool deleteMsg;					//消息撤回

	string relayGroupWord;			//发送到群 消息格式
	int relayGroupMsg_trimFront;	//发送到群 消息前几行删除
	int relayGroupMsg_trimBack;		//发送到群 消息后几行删除

	string keyWordGroupWarnWord;		//触发关键词群内警告
};

////设置
//struct Conf
//{
//	
//};

//map排序
struct CmpByKeySize {
	bool operator()(const int& k1, const int& k2)const {
		return k1 < k2;
	}
};

class MyJson
{
private:


	//写入json
	bool write_json(string path, Json::Value& root);

	//读取json
	bool read_json(string path, Json::Value& root);

	//获取key列表
	vector<string> getKeyList(vector<string> json_path);

	//解析json中的数组
	bool readJson_vector(vector<string> json_path, vector<string>& value);

	//解析json中的数组
	bool readJson_vector(vector<string> json_path, vector<long long>& value);

	//构造json中的数组
	bool writeJson_vector(vector<string> json_path, vector<string>& value);

	//构造json中的数组
	bool writeJson_vector(vector<string> json_path, vector<long long>& value);

	//获取json
	template <typename T>
	T get(vector<string> json_path, T def = T());


	//放置json
	template <typename T>
	void put(vector<string> json_path, T value);


public:


	//从keyword变量存放到json变量中
	bool keyWord2json();

	//从json变量存放到keyword变量中
	bool json2keyWord();

	//从REkeyword变量存放到json变量中
	bool keyWordRegex2json();

	//从json变量存放到REkeyword变量中
	bool json2keyWordRegex();

	//从keywordWhite变量存放到json变量中
	bool keyWordWhite2json();

	//从json变量存放到keywordWhite变量中
	bool json2keyWordWhite();

	//从 监控群名单 存放到json中的
	bool groupList2json();

	//从 json 存放到 监控群名单 中
	bool json2groupList();

	//从 QQ白名单/监控名单 存放到json中的
	bool QQlist2json();

	//从 json 存放到QQ白名单/监控名单中
	bool json2QQlist();

	//从 转发群名单 存放到json中的
	bool relayGroupList2json();

	//从 json 存放到 转发群名单 中
	bool json2relayGroupList();

	//从 root 存放到json中的
	bool admin2json();

	//从 json 存放到root中
	bool json2admin();

	//读取主要开关json到内存中
	bool json2mainSwitch();

	//从内存主要开关中放置的json
	bool mainSwitch2json();

	//读取群配置json到内存中
	bool json2aloneSwitch();

	//从群配置中放置到json
	bool aloneSwitch2json();


	bool json2alone()
	{
		try
		{
			auto keyList = getKeyList({ "alone" });

			for (auto temp_key : keyList)
			{
				auto& aloneTemp = alone[atoi(temp_key.c_str())];

				//普通关键词
				vector<string> temp_keyword;
				auto& keyWord = aloneTemp.keyWord;
				readJson_vector({ "alone",temp_key,"keyWord" }, temp_keyword);
				keyWord.clear();
				for (auto temp_keyWord : temp_keyword)
				{
					keyWord.push_back(temp_keyWord);
				}

				//正则表达式关键词
				vector<string> temp_keywordRegex;
				auto& keyWordRegex = aloneTemp.keyWordRegex;
				readJson_vector({ "alone",temp_key,"keyWordRegex" }, temp_keywordRegex);

				keyWordRegex.clear();
				for (auto temp_keyWordRegex : temp_keywordRegex)
				{
					keyWordRegex.push_back(temp_keyWordRegex);
				}

				//白名单关键词
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

				//单独设置
				if (temp.first != 0)
					temp_key = to_string(index++);

				//普通关键词
				vector<string> temp_keyword;
				for (auto temp_word : aloneTemp.keyWord)
				{
					temp_keyword.push_back(temp_word.keyWord);
				}

				writeJson_vector({ "alone", temp_key ,"keyWord" }, temp_keyword);

				//正则表达式关键词
				vector<string> temp_keyWordRegex;
				for (auto temp_wordRegex : aloneTemp.keyWordRegex)
				{
					temp_keyWordRegex.push_back(temp_wordRegex.keyWord);
				}

				writeJson_vector({ "alone", temp_key ,"keyWordRegex" }, temp_keyWordRegex);

				//白名单关键词
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








	//把所有的存放到json中的
	void all2json();

	//所有的json读取到内存中
	void json2all();

	//将json写到文件里
	void json2file();

	//文件中json读取出来
	void file2json();

public:

	//设置
	map<int, ConfAlone, CmpByKeySize> alone;		//单独设置
	vector<long long> admin;		//主人QQ
	string prefix;					//指令消息前缀
	bool relayPrivateMsg;			//消息转发

	//Json
	Json::Value conf_json;

};



class OperateLine
{
public:
	//获取 分割每行的字符串 json
	static void line_get_str(string str, vector<string>& value);

	//获取 分割每行的uint json
	static void line_get_ll(string str, vector<long long>& value);
};
