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
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

//��wstringת����string  
string OperateStr::wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
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

//�ַ���ȫ���滻
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


//д��json
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

#ifndef _DEBUG
		wbuilder["indentation"] = "";
#endif // !_DEBUG

		file << Json::writeString(wbuilder, root);

		file.close();
		return true;
	}
}

//��ȡjson
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

//����json�е�����
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


		value.clear();//���ԭ������

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

//����json�е�����
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


		value.clear();//���ԭ������

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

	return true;
}

//����json�е�����
bool MyJson::writeJson_vector(vector<string> json_path, vector<string>& value)
{
	try
	{
		Json::Value* temp_json = &conf_json;
		for (auto key : json_path)
		{
			temp_json = &((*temp_json)[key]);
		}


		temp_json->clear();//ɾ��ԭ����


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

//����json�е�����
bool MyJson::writeJson_vector(vector<string> json_path, vector<long long>& value)
{
	try
	{
		Json::Value* temp_json = &conf_json;
		for (auto key : json_path)
		{
			temp_json = &((*temp_json)[key]);
		}


		temp_json->clear();//ɾ��ԭ����


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

//��ȡkey�б�
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


//��ȡjson
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
		return temp_json.as<T>();

	return def;
}

//����json
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


//�� root ��ŵ�json�е�
bool MyJson::admin2json()
{
	writeJson_vector({ "main" ,"admin" }, admin);
	return true;
}

//�� json ��ŵ�root��

bool MyJson::json2admin()
{
	readJson_vector({ "main","admin" }, admin);

	return true;
}

//��ȡ��Ҫ����json���ڴ���
bool MyJson::json2mainSwitch()
{
	relayPrivateMsg = get<bool>({ "main","relayPrivateMsg" });
	prefix = get<string>({ "main","prefix" });

	return true;
}

//���ڴ���Ҫ�����з��õ�json
bool MyJson::mainSwitch2json()
{
	put<bool>({ "main","relayPrivateMsg" }, relayPrivateMsg);

	put<string>({ "main","prefix" }, prefix);

	return true;
}

//��ȡ�������õ��ڴ�
bool MyJson::json2alone()
{
	try
	{
		alone.clear();

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
			aloneTemp.use = get<bool>({ "alone",temp_key, "use" }, false);

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

//���������÷���json
bool MyJson::alone2json()
{

	try
	{

		conf_json["alone"].clear();

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
			put<bool>({ "alone",temp_key,"use" }, aloneTemp.use);

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
void MyJson::all2json()
{
	mainSwitch2json();
	admin2json();
	alone2json();
}

//���е�json��ȡ���ڴ���
void MyJson::json2all()
{
	json2mainSwitch();
	json2admin();
	json2alone();
}

//��jsonд���ļ���
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

//���ļ���json��ȡ����
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



//��ȡ �ָ�ÿ�е��ַ��� json
void OperateLine::line_get_str(string str, vector<string>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, str, boost::is_any_of(L"\n"));

	for (auto temp_str : temp_vectorStr)
	{
		//������Чֵ
		if (temp_str.empty())
			continue;

		value.push_back(temp_str);
	}
}

//��ȡ �ָ�ÿ�е�uint json
void OperateLine::line_get_ll(string str, vector<long long>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, str, boost::is_any_of(L"\n"));

	for (auto temp_str : temp_vectorStr)
	{
		long long num = atoll(temp_str.c_str());

		//������Чֵ
		if (num == 0)
			continue;

		value.push_back(num);
	}
}
