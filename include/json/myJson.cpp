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
bool g_MsgRelay;//��Ϣת��
string g_prefix;//ָ����Ϣǰ׺


#ifdef MFC

map<long long, string> WinGroupList;

#endif // MFC

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


//����json�е�����
bool OperateFile::json_json2vector_str(const wchar_t* json_path, vector<string>& value)
{
	try {
		value.clear();//���ԭ������


		//��ȡjson����
		auto op = conf_json.get_child_optional(json_path);

		//���û��key
		if (!op)
		{
			return false;
		}

		//��ӵ������� ת��string
		for (auto temp : op.get())
		{
			string str = OperateStr::wstring2string(temp.second.get_value<wstring>());

			//������Чֵ
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
		value.clear();//���ԭ������


		//��ȡjson����
		auto op = conf_json.get_child_optional(json_path);

		//���û��key
		if (!op)
		{
			return false;
		}

		//��ӵ������� ת��string
		for (auto temp : op.get())
		{
			long long num = temp.second.get_value<long long>();

			//������Чֵ
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


//����json�е�����
bool OperateFile::json_vector2json_str(const wchar_t* json_path, const wchar_t* json_after, vector<string>& value)
{
	try
	{
		wstring all_path = json_path;
		all_path += L".";
		all_path += json_after;

		conf_json.get_child(json_path).erase(json_after);//ɾ��ԭ����
		conf_json.add_child(all_path.c_str(), wptree(L""));//�����µ�

		//������� ת��wstring
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
		//����Ƿ������ӽ��
		//���û���Լ��½�һ��
		auto op = conf_json.get_child_optional(json_path);
		if (!op)
		{
			conf_json.add_child(json_path, wptree(L""));//�����µ�
		}

		wstring all_path = json_path;
		all_path += L".";
		all_path += json_after;

		conf_json.get_child(json_path).erase(json_after);//ɾ��ԭ����
		conf_json.add_child(all_path.c_str(), wptree(L""));//�����µ�

		//������� ת��wstring
		wptree child;
		for (auto temp : value)
		{
			//������Чֵ
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


//Ⱥ��������map ��ŵ�json

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


//Ⱥ��������json ��ȡmap
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



//��keyword������ŵ�json������
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

//��json������ŵ�keyword������
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

//��REkeyword������ŵ�json������
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

//��json������ŵ�REkeyword������
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

//��keywordWhite������ŵ�json������
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

//��json������ŵ�keywordWhite������
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

//�� ���Ⱥ���� ��ŵ�json�е�
bool OperateFile::GroupList2json()
{
	json_vector2json_longlong(L"Main", L"GroupList", g_GroupList);
	return true;
}

//�� json ��ŵ� ���Ⱥ���� ��
bool OperateFile::json2GroupList()
{
	json_json2vector_longlong(L"Main.GroupList", g_GroupList);

	return true;
}

//�� �������Ⱥ���� ��ŵ�json�е�
bool OperateFile::AlGroupList2json()
{
	json_vector2json_longlong(L"Main", L"AlGroupList", g_AlGroupList);
	return true;
}

//�� json ��ŵ� �������Ⱥ���� ��
bool OperateFile::json2AlGroupList()
{
	json_json2vector_longlong(L"Main.AlGroupList", g_AlGroupList);

	return true;
}

//�� QQ������/������� ��ŵ�json�е�
bool OperateFile::qqlist2json()
{
	json_map2json_longlong(L"QQList", g_qqlist);

	return true;
}

//�� json ��ŵ�QQ������/���������
bool OperateFile::json2qqlist()
{
	json_json2map_longlong(L"QQList", g_qqlist);

	return true;
}

//�� ����Ⱥ���� ��ŵ�json�е�
bool OperateFile::sendGroupList2json()
{
	json_map2json_longlong(L"SendGroupList", g_sendGroupList);

	return true;
}

//�� json ��ŵ� ����Ⱥ���� ��
bool OperateFile::json2sendGroupList()
{
	json_json2map_longlong(L"SendGroupList", g_sendGroupList);

	return true;
}

//�� root ��ŵ�json�е�
bool OperateFile::root2json()
{
	json_vector2json_longlong(L"Main", L"Root", g_root);
	return true;
}

//�� json ��ŵ�root��
bool OperateFile::json2root()
{
	json_json2vector_longlong(L"Main.Root", g_root);

	return true;
}

//��ȡ��Ҫ����json���ڴ���
bool OperateFile::json2main()
{
	g_MsgRelay = conf_json.get<bool>(L"Main.MsgRelay", false);

	wstring wstr_temp;

	wstr_temp = conf_json.get<wstring>(L"Main.prefix", L"");
	g_prefix = OperateStr::wstring2string(wstr_temp);

	return true;
}

//���ڴ���Ҫ�����з��õ�json
bool OperateFile::main2json()
{
	conf_json.put<bool>(L"Main.MsgRelay", g_MsgRelay);

	wstring wstr_temp;

	wstr_temp = OperateStr::string2wstring(g_prefix);
	conf_json.put<wstring>(L"Main.prefix", wstr_temp);

	return true;
}

//��ȡȺ����json���ڴ���
bool OperateFile::json2groupConf()
{
	try
	{
		auto op = conf_json.get_child_optional(L"GroupConf");

		//���û�д�·��
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

//��Ⱥ�����з��õ�json
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


//�����еĴ�ŵ�json�е�
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

//���е�json��ȡ���ڴ���
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

//��jsonд���ļ���
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

//���ļ���json��ȡ����
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
//���� �ָ�ÿ�е��ַ��� ����ӵ�json
void OperateLine::line_put_str(CString& str, vector<string>& value)
{
	value.clear();

	vector<string> temp_vector;
	boost::split(temp_vector, CStringA(str).GetBuffer(), boost::is_any_of(_T("\r\n")));

	//������Чֵ
	for (auto temp : temp_vector)
	{
		if (temp.empty())
			continue;

		value.push_back(temp);
	}

}

//���� �ָ�ÿ�е�uint ����ӵ�json
void OperateLine::line_put_ll(CString& str, vector<long long>& value)
{
	value.clear();

	vector<string> temp_vectorStr;
	boost::split(temp_vectorStr, CStringA(str).GetBuffer(), boost::is_any_of(_T("\r\n")));

	for (auto temp_str : temp_vectorStr)
	{
		long long num = atoll(temp_str.c_str());

		//������Чֵ
		if (num == 0)
			continue;

		value.push_back(num);
	}
}

//��ȡ �ָ�ÿ�е��ַ��� json
void OperateLine::line_get_str(CString& str, vector<string>& value)
{
	str = "";

	for (auto temp : value)
	{
		//������Чֵ
		if (temp.empty())
			continue;

		str += temp.c_str();
		str += "\r\n";
	}
}

//��ȡ �ָ�ÿ�е�uint json
void OperateLine::line_get_ll(CString& str, vector<long long>& value)
{
	str = "";

	for (auto temp : value)
	{
		//������Чֵ
		if (temp == 0)
			continue;

		str += to_string(temp).c_str();
		str += "\r\n";
	}

}

#else

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

#endif // MFC
