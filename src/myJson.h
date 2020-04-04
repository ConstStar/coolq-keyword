#pragma once
#include <vector>
#include <map>
#include <string>

#include <json/json.h>
#include <iostream>
#include <functional> 

using namespace std;

#ifdef _DEBUG
#pragma comment(lib,"jsoncpp_v142_Debug_MDd_x86.lib")
#else
#pragma comment(lib,"jsoncpp_v142_Release_MD_x86.lib")
#endif // _DEBUG


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
	bool use;						//开关
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
	bool keyWordPrivateWarn;		//触发后是否私聊警告
	bool streng;					//强力检测
	bool deleteMsg;					//消息撤回
	bool deleteCQCode;				//过滤CQ码

	string relayGroupWord;			//发送到群 消息格式
	int relayGroupMsg_trimFront;	//发送到群 消息前几行删除
	int relayGroupMsg_trimBack;		//发送到群 消息后几行删除

	string keyWordGroupWarnWord;		//触发关键词群内警告
	string keyWordPrivateWarnWord;		//触发关键词私聊警告
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

	//从 root 存放到json中的
	bool admin2json();

	//从 json 存放到root中
	bool json2admin();

	//读取主要开关json到内存中
	bool json2mainSwitch();

	//从内存主要开关中放置的json
	bool mainSwitch2json();

	//读取单独设置到内存
	bool json2alone();

	//将单独设置放入json
	bool alone2json();

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
