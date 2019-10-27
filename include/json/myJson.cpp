#pragma once

#include "myJson.h"

#include <Windows.h>

using namespace std;
using namespace boost::property_tree;


string AppFile;
wptree conf_json;


#ifdef MFC
map<long long, vector<string>> g_keyword;
map<long long, vector<string>> g_REKeyWord;
map<long long, vector<string>> g_KeyWordWhite;
#else
map<long long, vector<WKEYWORD>> g_keyword;
map<long long, vector<WKEYWORD>> g_REKeyWord;
map<long long, vector<WKEYWORD>> g_KeyWordWhite;
#endif // MFC


map<long long, vector<long long>> g_qqlist;
map<long long, vector<long long>> g_sendGroupList;
vector<long long> g_GroupList;
vector<long long> g_AlGroupList;
vector<long long> g_root;
map<long long, CONF> g_conf;
bool g_MsgRelay;//消息转发
string g_prefix;//指令消息前缀


#ifdef MFC

map<long long, string> WinGroupList;

#endif // MFC

wstring OperateStr::string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

//将wstring转换成string  
string OperateStr::wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;

}

string& OperateStr::replace_all(string& str, const  string& old_value, const  string& new_value)
{
	while (true)
	{
		string::size_type pos(0);
		if ((pos = str.find(old_value)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}

//字符串全部替换
string OperateStr::replace_all_distinct(string& str, string old_value, string new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}


//解析json中的数组
bool OperateFile::json_json2vector_str(const wchar_t* json_path, vector<string>& value)
{
	try {
		value.clear();//清空原有数组


		//获取json数组
		auto op = conf_json.get_child_optional(json_path);

		//如果没有key
		if (!op)
		{
			return false;
		}

		//添加到数组中 转成string
		for (auto temp : op.get())
		{
			string str = OperateStr::wstring2string(temp.second.get_value<wstring>());

			//过滤无效值
			if (str.empty())
				continue;

			value.push_back(str);
		}


	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}

	return true;
}

bool OperateFile::json_json2vector_longlong(const wchar_t* json_path, vector<long long>& value)
{
	try {
		value.clear();//清空原有数组


		//获取json数组
		auto op = conf_json.get_child_optional(json_path);

		//如果没有key
		if (!op)
		{
			return false;
		}

		//添加到数组中 转成string
		for (auto temp : op.get())
		{
			long long num = temp.second.get_value<long long>();

			//过滤无效值
			if (num == 0)
				continue;

			value.push_back(num);
		}


	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}

	return true;
}


//构造json中的数组
bool OperateFile::json_vector2json_str(const wchar_t* json_path, const wchar_t* json_after, vector<string>& value)
{
	try
	{
		wstring all_path = json_path;
		all_path += L".";
		all_path += json_after;

		conf_json.get_child(json_path).erase(json_after);//删除原来的
		conf_json.add_child(all_path.c_str(), wptree(L""));//构造新的

		//添加数组 转成wstring
		wptree child;
		for (auto temp : value)
		{
			if (temp.empty())
				continue;

			child.put<wstring>(L"", OperateStr::string2wstring(temp));
			conf_json.get_child(all_path.c_str()).push_back(make_pair(L"", child));
		}

	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}

bool OperateFile::json_vector2json_longlong(const wchar_t* json_path, const wchar_t* json_after, vector<long long>& value)
{
	try
	{
		//检测是否有主子结点
		//如果没有自己新建一个
		auto op = conf_json.get_child_optional(json_path);
		if (!op)
		{
			conf_json.add_child(json_path, wptree(L""));//构造新的
		}

		wstring all_path = json_path;
		all_path += L".";
		all_path += json_after;

		conf_json.get_child(json_path).erase(json_after);//删除原来的
		conf_json.add_child(all_path.c_str(), wptree(L""));//构造新的

		//添加数组 转成wstring
		wptree child;
		for (auto temp : value)
		{
			//过滤无效值
			if (temp == 0)
				continue;

			child.put<long long>(L"", temp);

			conf_json.get_child(all_path.c_str()).push_back(make_pair(L"", child));
		}

	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}


//群单独设置map 存放到json

bool OperateFile::json_map2json_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value)
{
	try
	{
		for (auto temp : value)
		{
			json_vector2json_longlong((L"GroupConf." + to_wstring(temp.first)).c_str(), json_after, temp.second);
		}

	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}

bool OperateFile::json_map2json_str(const wchar_t* json_after, map<long long, vector<string>>& value)
{
	try
	{
		for (auto temp : value)
		{
			json_vector2json_str((L"GroupConf." + to_wstring(temp.first)).c_str(), json_after, temp.second);

		}

	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}


//群单独设置json 读取map
bool OperateFile::json_json2map_str(const wchar_t* json_after, map<long long, vector<string>>& value)
{
	try
	{
		for (auto temp : conf_json.get_child(L"GroupConf"))
		{
			wstring temp_str;
			temp_str = L"GroupConf.";
			temp_str += temp.first.data();
			temp_str += L".";
			temp_str += json_after;

			json_json2vector_str(temp_str.c_str(), value[_wtoll(temp.first.data())]);

		}
	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}

bool OperateFile::json_json2map_longlong(const wchar_t* json_after, map<long long, vector<long long>>& value)
{
	try
	{
		for (auto temp : conf_json.get_child(L"GroupConf"))
		{
			wstring temp_str;
			temp_str = L"GroupConf.";
			temp_str += temp.first.data();
			temp_str += L".";
			temp_str += json_after;

			json_json2vector_longlong(temp_str.c_str(), value[_wtoll(temp.first.data())]);

		}
	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}
	return true;
}



//从keyword变量存放到json变量中
bool OperateFile::keyword2json()
{
#ifdef MFC

	json_map2json_str(L"keyword", g_keyword);

#else

	map<long long, vector<string>> temp_keyword;

	for (auto temp_key : g_keyword)
	{
		for (auto temp_word : temp_key.second)
		{
			temp_keyword[temp_key.first].push_back(temp_word.keyWord);
		}
	}


	json_map2json_str(L"keyword", temp_keyword);

#endif // MFC


	return true;
}

//从json变量存放到keyword变量中
bool OperateFile::json2keyword()
{
#ifdef MFC
	json_json2map_str(L"keyword", g_keyword);
#else
	g_keyword.clear();
	map<long long, vector<string>> temp_keyword;
	json_json2map_str(L"keyword", temp_keyword);

	for (auto temp_key : temp_keyword)
	{

		for (auto temp_word : temp_key.second)
		{

			WKEYWORD temp_wkeyword(temp_word);

			g_keyword[temp_key.first].push_back(temp_wkeyword);
		}

	}

#endif // MFC
	return true;
}

//从REkeyword变量存放到json变量中
bool OperateFile::REkeyword2json()
{

#ifdef MFC

	json_map2json_str(L"REKeyWord", g_REKeyWord);

#else

	map<long long, vector<string>> temp_keyword;

	for (auto temp_key : g_REKeyWord)
	{
		for (auto temp_word : temp_key.second)
		{
			temp_keyword[temp_key.first].push_back(temp_word.keyWord);
		}
	}


	json_map2json_str(L"REKeyWord", temp_keyword);

#endif // MFC
	return true;
}

//从json变量存放到REkeyword变量中
bool OperateFile::json2REkeyword()
{
#ifdef MFC
	json_json2map_str(L"REKeyWord", g_REKeyWord);
#else
	g_REKeyWord.clear();
	map<long long, vector<string>> temp_keyword;
	json_json2map_str(L"REKeyWord", temp_keyword);

	for (auto temp_key : temp_keyword)
	{

		for (auto temp_word : temp_key.second)
		{

			WKEYWORD temp_wkeyword(temp_word);

			g_REKeyWord[temp_key.first].push_back(temp_wkeyword);
		}

	}

#endif // MFC
	return true;
}

//从keywordWhite变量存放到json变量中
bool OperateFile::keywordWhite2json()
{
#ifdef MFC

	json_map2json_str(L"KeyWordWhite", g_KeyWordWhite);

#else

	map<long long, vector<string>> temp_keyword;

	for (auto temp_key : g_KeyWordWhite)
	{
		for (auto temp_word : temp_key.second)
		{
			temp_keyword[temp_key.first].push_back(temp_word.keyWord);
		}
	}


	json_map2json_str(L"KeyWordWhite", temp_keyword);

#endif // MFC
	return true;
}

//从json变量存放到keywordWhite变量中
bool OperateFile::json2keywordWhite()
{
#ifdef MFC
	json_json2map_str(L"KeyWordWhite", g_KeyWordWhite);
#else
	g_KeyWordWhite.clear();
	map<long long, vector<string>> temp_keyword;
	json_json2map_str(L"KeyWordWhite", temp_keyword);

	for (auto temp_key : temp_keyword)
	{

		for (auto temp_word : temp_key.second)
		{

			WKEYWORD temp_wkeyword(temp_word);

			g_KeyWordWhite[temp_key.first].push_back(temp_wkeyword);
		}

	}

#endif // MFC
	return true;
}

//从 监控群名单 存放到json中的
bool OperateFile::GroupList2json()
{
	json_vector2json_longlong(L"Main", L"GroupList", g_GroupList);
	return true;
}

//从 json 存放到 监控群名单 中
bool OperateFile::json2GroupList()
{
	json_json2vector_longlong(L"Main.GroupList", g_GroupList);

	return true;
}

//从 单独监控群名单 存放到json中的
bool OperateFile::AlGroupList2json()
{
	json_vector2json_longlong(L"Main", L"AlGroupList", g_AlGroupList);
	return true;
}

//从 json 存放到 单独监控群名单 中
bool OperateFile::json2AlGroupList()
{
	json_json2vector_longlong(L"Main.AlGroupList", g_AlGroupList);

	return true;
}

//从 QQ白名单/监控名单 存放到json中的
bool OperateFile::qqlist2json()
{
	json_map2json_longlong(L"QQList", g_qqlist);

	return true;
}

//从 json 存放到QQ白名单/监控名单中
bool OperateFile::json2qqlist()
{
	json_json2map_longlong(L"QQList", g_qqlist);

	return true;
}

//从 发送群名单 存放到json中的
bool OperateFile::sendGroupList2json()
{
	json_map2json_longlong(L"SendGroupList", g_sendGroupList);

	return true;
}

//从 json 存放到 发送群名单 中
bool OperateFile::json2sendGroupList()
{
	json_json2map_longlong(L"SendGroupList", g_sendGroupList);

	return true;
}

//从 root 存放到json中的
bool OperateFile::root2json()
{
	json_vector2json_longlong(L"Main", L"Root", g_root);
	return true;
}

//从 json 存放到root中
bool OperateFile::json2root()
{
	json_json2vector_longlong(L"Main.Root", g_root);

	return true;
}

//读取主要配置json到内存中
bool OperateFile::json2main()
{
	g_MsgRelay = conf_json.get<bool>(L"Main.MsgRelay", false);

	wstring wstr_temp;

	wstr_temp = conf_json.get<wstring>(L"Main.prefix", L"");
	g_prefix = OperateStr::wstring2string(wstr_temp);

	return true;
}

//从内存主要配置中放置到json
bool OperateFile::main2json()
{
	conf_json.put<bool>(L"Main.MsgRelay", g_MsgRelay);

	wstring wstr_temp;

	wstr_temp = OperateStr::string2wstring(g_prefix);
	conf_json.put<wstring>(L"Main.prefix", wstr_temp);

	return true;
}

//读取群配置json到内存中
bool OperateFile::json2groupConf()
{
	try
	{
		auto op = conf_json.get_child_optional(L"GroupConf");

		//如果没有此路径
		if (!op)
			return false;

		for (auto temp_key : op.get())
		{
			//bool
			g_conf[_wtoll(temp_key.first.data())].GroupWarn = temp_key.second.get<bool>(L"GroupWarn", false);
			g_conf[_wtoll(temp_key.first.data())].Revoke = temp_key.second.get<bool>(L"Revoke", false);
			g_conf[_wtoll(temp_key.first.data())].Streng = temp_key.second.get<bool>(L"Streng", false);

			//int
			g_conf[_wtoll(temp_key.first.data())].sendGroupMsg_frontLine = temp_key.second.get<int>(L"SendGroupMsg_FrontLine", 0);
			g_conf[_wtoll(temp_key.first.data())].sendGroupMsg_afterLine = temp_key.second.get<int>(L"SendGroupMsg_AfterLine", 0);
			g_conf[_wtoll(temp_key.first.data())].typeQQList = temp_key.second.get<int>(L"TypeQQList", 0);
			g_conf[_wtoll(temp_key.first.data())].way = temp_key.second.get<int>(L"Way", 0);
			g_conf[_wtoll(temp_key.first.data())].wayTime = temp_key.second.get<int>(L"WayTime", 0);

			//str
			g_conf[_wtoll(temp_key.first.data())].sendGroupMsg_word = OperateStr::wstring2string(temp_key.second.get<wstring>(L"SendGroupMsg_Word", L""));
			g_conf[_wtoll(temp_key.first.data())].WordKeyWarn = OperateStr::wstring2string(temp_key.second.get<wstring>(L"WordKeyWarn", L""));

		}
	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}

	return true;
}

//从群配置中放置到json
bool OperateFile::groupConf2json()
{
	try
	{
		for (auto temp_key : g_conf)
		{
			wstring wstr_temp;

			//bool
			conf_json.put<bool>(L"GroupConf." + to_wstring(temp_key.first) + L".GroupWarn", temp_key.second.GroupWarn);
			conf_json.put<bool>(L"GroupConf." + to_wstring(temp_key.first) + L".Revoke", temp_key.second.Revoke);
			conf_json.put<bool>(L"GroupConf." + to_wstring(temp_key.first) + L".Streng", temp_key.second.Streng);


			//int
			conf_json.put<int>(L"GroupConf." + to_wstring(temp_key.first) + L".SendGroupMsg_FrontLine", temp_key.second.sendGroupMsg_frontLine);
			conf_json.put<int>(L"GroupConf." + to_wstring(temp_key.first) + L".SendGroupMsg_AfterLine", temp_key.second.sendGroupMsg_afterLine);
			conf_json.put<int>(L"GroupConf." + to_wstring(temp_key.first) + L".TypeQQList", temp_key.second.typeQQList);
			conf_json.put<int>(L"GroupConf." + to_wstring(temp_key.first) + L".Way", temp_key.second.way);
			conf_json.put<int>(L"GroupConf." + to_wstring(temp_key.first) + L".WayTime", temp_key.second.wayTime);


			//str
			wstr_temp = OperateStr::string2wstring(temp_key.second.sendGroupMsg_word);
			conf_json.put<wstring>(L"GroupConf." + to_wstring(temp_key.first) + L".SendGroupMsg_Word", wstr_temp);

			wstr_temp = OperateStr::string2wstring(temp_key.second.WordKeyWarn);
			conf_json.put<wstring>(L"GroupConf." + to_wstring(temp_key.first) + L".WordKeyWarn", wstr_temp);


		}
	}
	catch (exception & e)
	{
#ifdef DEBUG

		cout << e.what() << endl;

#endif // DEBUG
		return false;
	}

	return true;
}


//把所有的存放到json中的
void OperateFile::all2json()
{
	//conf_json.clear();
	main2json();
	root2json();
	qqlist2json();
	AlGroupList2json();
	GroupList2json();
	keywordWhite2json();
	REkeyword2json();
	keyword2json();
	sendGroupList2json();
	groupConf2json();
}

//所有的json读取到内存中
void OperateFile::json2all()
{
	json2main();
	json2root();
	json2qqlist();
	json2GroupList();
	json2AlGroupList();
	json2keywordWhite();
	json2REkeyword();
	json2keyword();
	json2sendGroupList();
	json2groupConf();
}

//将json写到文件里
void OperateFile::json2file()
{
	try
	{
		write_json((AppFile + "conf.json"), conf_json);
	}
	catch (exception & e)
	{
#ifdef DEBUG
		cout << e.what() << endl;
#endif
	}
}

//将文件的json读取出来
void OperateFile::file2json()
{
	try
	{
		read_json((AppFile + "conf.json"), conf_json);
	}
	catch (exception & e)
	{
#ifdef DEBUG
		cout << e.what() << endl;
#endif
	}
}

#ifdef MFC

void OperateFile::json2WinGroupList(const char* GroupList_str)
{
	try
	{
		wptree GroupList_json;

		std::wstringstream ss(OperateStr::string2wstring(string(GroupList_str)).c_str());
		read_json(ss, GroupList_json);

		for (auto temp : GroupList_json)
		{
			WinGroupList[_wtoll(temp.first.data())] = OperateStr::wstring2string(wstring(temp.second.data()));
		}
	}
	catch (exception & e)
	{
#ifdef DEBUG
		cout << "GroupList_str :\n" << GroupList_str << endl;
		cout << e.what() << endl;

#endif // DEBUG

	}
}


#endif // MFC


#ifdef MFC
//放置 分割每行的字符串 并添加到json
void OperateLine::line_put_str(CString& str, vector<string>& value)
{
	value.clear();

	vector<string> temp_vector;
	boost::split(temp_vector, CStringA(str).GetBuffer(), boost::is_any_of(_T("\r\n")));

	//过滤无效值
	for (auto temp : temp_vector)
	{
		if (temp.empty())
			continue;

		value.push_back(temp);
	}

}

//放置 分割每行的uint 并添加到json
void OperateLine::line_put_ll(CString& str, vector<long long>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, CStringA(str).GetBuffer(), boost::is_any_of(_T("\r\n")));

	for (auto temp_str : temp_vectorStr)
	{
		long long num = atoll(temp_str.c_str());

		//过滤无效值
		if (num == 0)
			continue;

		value.push_back(num);
	}
}

//获取 分割每行的字符串 json
void OperateLine::line_get_str(CString& str, vector<string>& value)
{
	str = "";

	for (auto temp : value)
	{
		//过滤无效值
		if (temp.empty())
			continue;

		str += temp.c_str();
		str += "\r\n";
	}
}

//获取 分割每行的uint json
void OperateLine::line_get_ll(CString& str, vector<long long>& value)
{
	str = "";

	for (auto temp : value)
	{
		//过滤无效值
		if (temp == 0)
			continue;

		str += to_string(temp).c_str();
		str += "\r\n";
	}

}

#else

//获取 分割每行的字符串 json
void OperateLine::line_get_str(string str, vector<string>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, str, boost::is_any_of(L"\n"));

	for (auto temp_str : temp_vectorStr)
	{
		//过滤无效值
		if (temp_str.empty())
			continue;

		value.push_back(temp_str);
	}
}

//获取 分割每行的uint json
void OperateLine::line_get_ll(string str, vector<long long>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, str, boost::is_any_of(L"\n"));

	for (auto temp_str : temp_vectorStr)
	{
		long long num = atoll(temp_str.c_str());

		//过滤无效值
		if (num == 0)
			continue;

		value.push_back(num);
	}
}

#endif // MFC
