#pragma once

#include <Windows.h>
#include <fstream>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "myJson.h"


using namespace std;

extern string appDir;


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


//写入json
bool MyJson::write_json(string path, Json::Value& root)
{
	ofstream file;
	file.open(path);
	if (!file.good())
	{
		return false;
	}
	else
	{
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "";

		file << Json::writeString(wbuilder, root);

		file.close();
		return true;
	}
}

//读取json
bool MyJson::read_json(string path, Json::Value& root)
{
	ifstream file;
	file.open(path);
	if (!file.good())
	{
		return false;
	}
	else
	{
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		bool ok = Json::parseFromStream(rbuilder, file, &root, &errs);

		if (!ok || !errs.empty())
			return false;

		file.close();
		return true;
	}
}

//解析json中的数组
bool MyJson::readJson_vector(vector<string> json_path, vector<string>& value)
{
	try {

		Json::Value temp_json = conf_json;
		for (auto key : json_path)
		{
			temp_json = temp_json[key];

			if (temp_json.isNull())
			{
				return false;
			}
		}

		if (!temp_json.isArray())
		{
			return false;
		}


		value.clear();//清空原有数组

		for (auto temp : temp_json)
		{
			value.push_back(temp.asString());
		}

	}
	catch (exception & e)
	{
		cout << e.what() << endl;

		return false;
	}

	return true;
}

//解析json中的数组
bool MyJson::readJson_vector(vector<string> json_path, vector<long long>& value)
{
	try {

		Json::Value temp_json = conf_json;
		for (auto key : json_path)
		{
			temp_json = temp_json[key];

			if (temp_json.isNull())
			{
				return false;
			}
		}

		if (!temp_json.isArray())
		{
			return false;
		}


		value.clear();//清空原有数组

		for (auto temp : temp_json)
		{
			value.push_back(temp.asInt64());
		}
	}
	catch (exception & e)
	{

		cout << e.what() << endl;

		return false;
	}
}

//构造json中的数组
bool MyJson::writeJson_vector(vector<string> json_path, vector<string>& value)
{
	try
	{
		Json::Value* temp_json = &conf_json;
		for (auto key : json_path)
		{
			temp_json = &((*temp_json)[key]);
		}


		temp_json->clear();//删除原来的


		for (auto temp : value)
		{
			if (temp.empty())
				continue;

			temp_json->append(temp);
		}

	}
	catch (exception & e)
	{

		cout << e.what() << endl;

		return false;
	}
	return true;
}

//构造json中的数组
bool MyJson::writeJson_vector(vector<string> json_path, vector<long long>& value)
{
	try
	{
		Json::Value* temp_json = &conf_json;
		for (auto key : json_path)
		{
			temp_json = &((*temp_json)[key]);
		}


		temp_json->clear();//删除原来的


		for (auto temp : value)
		{
			temp_json->append(temp);
		}

	}
	catch (exception & e)
	{

		cout << e.what() << endl;

		return false;
	}
	return true;
}

//获取key列表
vector<string> MyJson::getKeyList(vector<string> json_path)
{
	Json::Value temp_json = conf_json;
	vector<string> keyList;

	for (auto key : json_path)
	{
		temp_json = temp_json[key];

		if (temp_json.isNull())
		{
			return keyList;
		}
	}

	keyList = temp_json.getMemberNames();

	return keyList;
}


//获取json
template <typename T>
T MyJson::get(vector<string> json_path, T def)
{
	Json::Value temp_json = conf_json;
	for (auto key : json_path)
	{
		temp_json = temp_json[key];

		if (temp_json.isNull())
		{
			return def;
		}
	}

	if (temp_json.is<T>())
		temp_json.as<T>();

	return def;
}

//放置json
template <typename T>
void MyJson::put(vector<string> json_path, T value)
{
	Json::Value* temp_json = &conf_json;
	for (auto key : json_path)
	{
		temp_json = &((*temp_json)[key]);
	}

	(*temp_json) = value;

}

//从keyword变量存放到json变量中
bool MyJson::keyWord2json()
{
	for (auto& temp : alone)
	{
		vector<string> temp_keyword;
		for (auto temp_word : temp.second.keyWord)
		{
			temp_keyword.push_back(temp_word.keyWord);
		}

		writeJson_vector({ "alone", to_string(temp.first) ,"keyWord" }, temp_keyword);

	}

	return true;
}

//从json变量存放到keyword变量中
bool MyJson::json2keyWord()
{

	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		vector<string> temp_keyword;
		auto& keyWord = alone[atoll(temp_key.c_str())].keyWord;
		readJson_vector({ "alone",temp_key,"keyWord" }, temp_keyword);

		keyWord.clear();
		for (auto temp_keyWord : temp_keyword)
		{
			keyWord.push_back(temp_keyWord);
		}

	}
	return true;
}

//从REkeyword变量存放到json变量中
bool MyJson::keyWordRegex2json()
{

	for (auto& temp : alone)
	{
		vector<string> temp_keyWordRegex;
		for (auto temp_wordRegex : temp.second.keyWordRegex)
		{
			temp_keyWordRegex.push_back(temp_wordRegex.keyWord);
		}

		writeJson_vector({ "alone", to_string(temp.first) ,"keyWordRegex" }, temp_keyWordRegex);

	}

	return true;
}

//从json变量存放到REkeyword变量中
bool MyJson::json2keyWordRegex()
{

	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		vector<string> temp_keywordRegex;
		auto& keyWordRegex = alone[atoll(temp_key.c_str())].keyWordRegex;
		readJson_vector({ "alone",temp_key,"keyWordRegex" }, temp_keywordRegex);

		keyWordRegex.clear();
		for (auto temp_keyWordRegex : temp_keywordRegex)
		{
			keyWordRegex.push_back(temp_keyWordRegex);
		}

	}
	return true;
}

//从keywordWhite变量存放到json变量中
bool MyJson::keyWordWhite2json()
{

	for (auto& temp : alone)
	{
		vector<string> temp_keyWordWhite;
		for (auto temp_wordWhite : temp.second.keyWordWhite)
		{
			temp_keyWordWhite.push_back(temp_wordWhite.keyWord);
		}

		writeJson_vector({ "alone", to_string(temp.first) ,"keyWordWhite" }, temp_keyWordWhite);

	}

	return true;
}

//从json变量存放到keywordWhite变量中
bool MyJson::json2keyWordWhite()
{
	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		vector<string> temp_keywordWhite;
		auto& keyWordWhite = alone[atoll(temp_key.c_str())].keyWordWhite;
		readJson_vector({ "alone",temp_key,"keyWordWhite" }, temp_keywordWhite);

		keyWordWhite.clear();
		for (auto temp_keyWordRegex : temp_keywordWhite)
		{
			keyWordWhite.push_back(temp_keyWordRegex);
		}

	}

	return true;
}

//从 监控群名单 存放到json中的
bool MyJson::groupList2json()
{
	for (auto& temp : alone)
	{
		writeJson_vector({ "alone",to_string(temp.first),"groupList" }, alone[temp.first].groupList);
	}

	return true;
}

//从 json 存放到 监控群名单 中
bool MyJson::json2groupList()
{
	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		auto& groupList = alone[atoll(temp_key.c_str())].groupList;
		readJson_vector({ "alone",temp_key,"groupList" }, groupList);
	}

	return true;
}

//从 QQ白名单/监控名单 存放到json中的
bool MyJson::QQlist2json()
{
	for (auto& temp : alone)
	{
		writeJson_vector({ "alone",to_string(temp.first),"QQList" }, alone[temp.first].QQList);
	}

	return true;
}

//从 json 存放到QQ白名单/监控名单中
bool MyJson::json2QQlist()
{
	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		auto& QQList = alone[atoll(temp_key.c_str())].QQList;
		readJson_vector({ "alone",temp_key,"QQList" }, QQList);
	}

	return true;
}

//从 转发群名单 存放到json中的
bool MyJson::relayGroupList2json()
{

	for (auto& temp : alone)
	{
		writeJson_vector({ "alone",to_string(temp.first),"relayGroupList" }, alone[temp.first].relayGroupList);
	}

	return true;
}

//从 json 存放到 转发群名单 中
bool MyJson::json2relayGroupList()
{
	auto keyList = getKeyList({ "alone" });

	for (auto temp_key : keyList)
	{
		auto& relayGroupList = alone[atoll(temp_key.c_str())].relayGroupList;
		readJson_vector({ "alone",temp_key,"relayGroupList" }, relayGroupList);
	}

	return true;
}

//从 root 存放到json中的
bool MyJson::admin2json()
{
	writeJson_vector({ "Main" ,"Root" }, admin);
	return true;
}

//从 json 存放到root中
bool MyJson::json2admin()
{
	readJson_vector({ "Main","Root" }, admin);

	return true;
}

//读取主要配置json到内存中
bool MyJson::json2main()
{
	msgRelay = get<bool>({ "main","msgRelay" });
	prefix = get<string>({ "main","prefix" });

	return true;
}

//从内存主要配置中放置到json
bool MyJson::main2json()
{
	put<bool>({ "main","msgRelay" }, msgRelay);

	put<string>({ "main","prefix" }, prefix);

	return true;
}

//读取群配置json到内存中
bool MyJson::json2groupConf()
{
	try
	{

		auto keyList = getKeyList({ "alone" });

		for (auto temp_key : keyList)
		{
			auto& aloneTemp = alone[atoi(temp_key.c_str())];


			aloneTemp.groupWarn = get<bool>({ "alone",temp_key, "groupWarn" }, false);
			aloneTemp.revoke = get<bool>({ "alone",temp_key, "revoke" }, false);
			aloneTemp.streng = get<bool>({ "alone",temp_key, "streng" }, false);

			aloneTemp.relayGroupMsg_afterLine = get<int>({ "alone",temp_key, "relayGroupMsg_afterLine" }, 0);
			aloneTemp.relayGroupMsg_frontLine = get<int>({ "alone",temp_key, "relayGroupMsg_frontLine" }, 0);
			aloneTemp.QQListType = get<int>({ "alone",temp_key, "QQListType" }, 0);
			aloneTemp.dealType = get<int>({ "alone",temp_key, "dealType" }, 0);
			aloneTemp.banTimeLen = get<int>({ "alone",temp_key, "banTimeLen" }, 0);

			aloneTemp.relayGroupWord = get<string>({ "alone",temp_key, "relayGroupWord" }, "");
			aloneTemp.keyWordGroupWarn = get<string>({ "alone",temp_key, "keyWordGroupWarn" }, "");
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
bool MyJson::groupConf2json()
{
	try
	{
		for (auto& temp_alone : alone)
		{
			string temp_key = to_string(temp_alone.first);
			auto& aloneTemp = alone[temp_alone.first];

			put<bool>({ "alone",temp_key,"groupWarn" }, aloneTemp.groupWarn);
			put<bool>({ "alone",temp_key,"revoke" }, aloneTemp.revoke);
			put<bool>({ "alone",temp_key,"streng" }, aloneTemp.streng);

			put<int>({ "alone",temp_key,"relayGroupMsg_afterLine" }, aloneTemp.relayGroupMsg_afterLine);
			put<int>({ "alone",temp_key,"relayGroupMsg_frontLine" }, aloneTemp.relayGroupMsg_frontLine);
			put<int>({ "alone",temp_key,"QQListType" }, aloneTemp.QQListType);
			put<int>({ "alone",temp_key,"dealType" }, aloneTemp.dealType);
			put<int>({ "alone",temp_key,"banTimeLen" }, aloneTemp.banTimeLen);

			put<string>({ "alone",temp_key,"relayGroupWord" }, aloneTemp.relayGroupWord);
			put<string>({ "alone",temp_key,"keyWordGroupWarn" }, aloneTemp.keyWordGroupWarn);

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
void MyJson::all2json()
{
	//conf_json.clear();
	main2json();
	admin2json();
	QQlist2json();
	groupList2json();
	keyWordWhite2json();
	keyWordRegex2json();
	keyWord2json();
	relayGroupList2json();
	groupConf2json();
}

//所有的json读取到内存中
void MyJson::json2all()
{
	json2main();
	json2admin();
	json2QQlist();
	json2groupList();
	json2keyWordWhite();
	json2keyWordRegex();
	json2keyWord();
	json2relayGroupList();
	json2groupConf();
}

//将json写到文件里
void MyJson::json2file()
{
	try
	{
		write_json((appDir + "conf.json"), conf_json);
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
	}
}

//将文件的json读取出来
void MyJson::file2json()
{
	try
	{
		read_json((appDir + "conf.json"), conf_json);
	}
	catch (exception & e)
	{
#ifdef DEBUG
		cout << e.what() << endl;
#endif
	}
}



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
