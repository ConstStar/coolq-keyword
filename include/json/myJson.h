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
	int way;//处理方法
	int wayTime;//禁言时间
	int typeQQList;//名单类型  白名单/监控名单
	bool GroupWarn;//触发后是否群内警告
	bool Streng;//强力检测
	bool Revoke;//消息撤回

	string sendGroupMsg_word;//发送到群 消息格式
	int sendGroupMsg_frontLine;//发送到群 消息前几行删除
	int sendGroupMsg_afterLine;//发送到群 消息后几行删除

	string WordKeyWarn;//触发关键词群内提醒内容
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
extern bool g_MsgRelay;//消息转发
extern string g_prefix;//指令消息前缀

#ifdef MFC

extern map<long long, string> WinGroupList;

#endif // MFC


class OperateFile
{

public:

	//解析json中的数组
	static bool json_json2vector_str(const wchar_t* json_path, vector<string>& value);

	static bool json_json2vector_longlong(const wchar_t* json_path, vector<long long>& value);


	//构造json中的数组
	static bool json_vector2json_str(const wchar_t* json_path, const wchar_t* json_after, vector<string>& value);


	static bool json_vector2json_longlong(const wchar_t* json_path, const wchar_t* json_after, vector<long long>& value);


	//群单独设置map 存放到json

	static bool json_map2json_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value);


	static bool json_map2json_str(const wchar_t* json_after, map<long long, vector<string>>& value);


	//群单独设置json 读取map
	static bool json_json2map_str(const wchar_t* json_after, map<long long, vector<string>>& value);


	static bool json_json2map_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value);



	//从keyword变量存放到json变量中
	static bool keyword2json();

	//从json变量存放到keyword变量中
	static bool json2keyword();

	//从REkeyword变量存放到json变量中
	static bool REkeyword2json();

	//从json变量存放到REkeyword变量中
	static bool json2REkeyword();

	//从keywordWhite变量存放到json变量中
	static bool keywordWhite2json();

	//从json变量存放到keywordWhite变量中
	static bool json2keywordWhite();

	//从 监控群名单 存放到json中的
	static bool GroupList2json();

	//从 json 存放到 监控群名单 中
	static bool json2GroupList();

	//从 单独监控群名单 存放到json中的
	static bool AlGroupList2json();

	//从 json 存放到 单独监控群名单 中
	static bool json2AlGroupList();

	//从 QQ白名单/监控名单 存放到json中的
	static bool qqlist2json();

	//从 json 存放到QQ白名单/监控名单中
	static bool json2qqlist();

	//从 发送群名单 存放到json中的
	static bool sendGroupList2json();

	//从 json 存放到 发送群名单 中
	static bool json2sendGroupList();

	//从 root 存放到json中的
	static bool root2json();

	//从 json 存放到root中
	static bool json2root();

	//读取主要配置json到内存中
	static bool json2main();

	//从内存主要配置中放置的json
	static bool main2json();

	//读取群配置json到内存中
	static bool json2groupConf();

	//从群配置中放置到json
	static bool groupConf2json();



	//把所有的存放到json中的
	static void all2json();

	//所有的json读取到内存中
	static void json2all();

	//将json写到文件里
	static void json2file();

	//文件中json读取出来
	static void file2json();

#ifdef MFC

	static void json2WinGroupList(const char* GroupList_str);


#endif // MFC

};

class OperateLine
{
public:
#ifdef MFC
	//放置 分割每行的字符串 并添加到json
	static void line_put_str(CString& str, vector<string>& value);

	//放置 分割每行的uint 并添加到json
	static void line_put_ll(CString& str, vector<long long>& value);

	//获取 分割每行的字符串 json
	static void line_get_str(CString& str, vector<string>& value);

	//获取 分割每行的uint json
	static void line_get_ll(CString& str, vector<long long>& value);
#else

	//获取 分割每行的字符串 json
	static void line_get_str(string str, vector<string>& value);

	//获取 分割每行的uint json
	static void line_get_ll(string str, vector<long long>& value);


#endif // MFC
};
