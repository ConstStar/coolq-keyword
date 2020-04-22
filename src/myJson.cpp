#include "myJson.h"
#include "mycq.hpp"

// #include <Windows.h>
#include <fstream>

// #include <boost/algorithm/string.hpp>
// #include <boost/format.hpp>
// #include <boost/tokenizer.hpp>

using namespace std;

extern string appDir;

wstring OperateStr::string2wstring(string str) {
    // wstring result;
    // //获取缓冲区大小，并申请空间，缓冲区大小按字符计算
    // int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    // TCHAR* buffer = new TCHAR[len + 1];
    // //多字节编码转换成宽字节编码
    // MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    // buffer[len] = '\0';             //添加字符串结尾
    // //删除缓冲区并返回值
    // result.append(buffer);
    // delete[] buffer;
    // return result;

    return cq::utils::s2ws(str);
}

//将wstring转换成string
string OperateStr::wstring2string(wstring wstr) {
    // string result;
    // //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
    // int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    // char* buffer = new char[len + 1];
    // //宽字节编码转换成多字节编码
    // WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    // buffer[len] = '\0';
    // //删除缓冲区并返回值
    // result.append(buffer);
    // delete[] buffer;
    // return result;

    return cq::utils::ws2s(wstr);
}

string& OperateStr::replace_all(string& str, const string& old_value, const string& new_value) {
    while (true) {
        string::size_type pos(0);
        if ((pos = str.find(old_value)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}

//字符串全部替换
string OperateStr::replace_all_distinct(string& str, string old_value, string new_value) {
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}

//写入json
bool MyJson::write_json(string path, Json::Value& root) {
    path = cq::utils::ansi(path);
    ofstream file;
    file.open(path);
    if (!file.good()) {
        return false;
    } else {
        Json::StreamWriterBuilder wbuilder;

#ifndef _DEBUG
        wbuilder["indentation"] = "";
#endif // !_DEBUG

        file << Json::writeString(wbuilder, root);

        file.close();
        return true;
    }
}

//读取json
bool MyJson::read_json(string path, Json::Value& root) {
    path = cq::utils::ansi(path);
    ifstream file;
    file.open(path);
    if (!file.good()) {
        return false;
    } else {
        Json::CharReaderBuilder rbuilder;
        rbuilder["collectComments"] = false;
        std::string errs;
        bool ok = Json::parseFromStream(rbuilder, file, &root, &errs);

        if (!ok || !errs.empty()) return false;

        file.close();
        return true;
    }
}

//解析json中的数组
bool MyJson::readJson_array(const initializer_list<string> json_path, unordered_set<string>& value) {
    try {
        Json::Value temp_json = conf_json;
        for (auto key : json_path) {
            temp_json = temp_json[key];

            if (temp_json.isNull()) {
                return false;
            }
        }

        if (!temp_json.isArray()) {
            return false;
        }

        value.clear(); //清空原有数组

        for (auto temp : temp_json) {
            value.insert(temp.asString());
        }

    } catch (exception& e) {
        cout << e.what() << endl;

        return false;
    }

    return true;
}

//解析json中的数组
bool MyJson::readJson_array(const initializer_list<string> json_path, unordered_set<long long>& value) {
    try {
        Json::Value temp_json = conf_json;
        for (auto key : json_path) {
            temp_json = temp_json[key];

            if (temp_json.isNull()) {
                return false;
            }
        }

        if (!temp_json.isArray()) {
            return false;
        }

        value.clear(); //清空原有数组

        for (auto temp : temp_json) {
            value.insert(temp.asInt64());
        }
    } catch (exception& e) {
        cout << e.what() << endl;

        return false;
    }

    return true;
}

//构造json中的数组
bool MyJson::writeJson_array(const initializer_list<string> json_path, unordered_set<string>& value) {
    try {
        Json::Value* temp_json = &conf_json;
        for (auto key : json_path) {
            temp_json = &((*temp_json)[key]);
        }

        temp_json->clear(); //删除原来的

        for (auto temp : value) {
            if (temp.empty()) continue;

            temp_json->append(temp);
        }

    } catch (exception& e) {
        cout << e.what() << endl;

        return false;
    }
    return true;
}

//构造json中的数组
bool MyJson::writeJson_array(const initializer_list<string> json_path, unordered_set<long long>& value) {
    try {
        Json::Value* temp_json = &conf_json;
        for (auto key : json_path) {
            temp_json = &((*temp_json)[key]);
        }

        temp_json->clear(); //删除原来的

        for (auto temp : value) {
            temp_json->append(temp);
        }

    } catch (exception& e) {
        cout << e.what() << endl;

        return false;
    }
    return true;
}

//获取key列表
vector<string> MyJson::getKeyList(const initializer_list<string> json_path) {
    Json::Value temp_json = conf_json;
    vector<string> keyList;

    for (auto key : json_path) {
        temp_json = temp_json[key];

        if (temp_json.isNull()) {
            return keyList;
        }
    }

    keyList = temp_json.getMemberNames();

    return keyList;
}

//获取json
template <typename T>
T MyJson::get(const initializer_list<string> json_path, T def) {
    Json::Value temp_json = conf_json;
    for (auto key : json_path) {
        temp_json = temp_json[key];

        if (temp_json.isNull()) {
            return def;
        }
    }

    if (temp_json.is<T>()) return temp_json.as<T>();

    return def;
}

//放置json
template <typename T>
void MyJson::put(const initializer_list<string> json_path, T value) {
    Json::Value* temp_json = &conf_json;
    for (auto key : json_path) {
        temp_json = &((*temp_json)[key]);
    }

    (*temp_json) = value;
}

//从 root 存放到json中的
bool MyJson::admin2json() {
    writeJson_array({"main", "admin"}, admin);
    return true;
}

//从 json 存放到root中

bool MyJson::json2admin() {
    readJson_array({"main", "admin"}, admin);

    return true;
}

//读取主要配置json到内存中
bool MyJson::json2mainSwitch() {
    relayPrivateMsg = get<bool>({"main", "relayPrivateMsg"});
    prefix = get<string>({"main", "prefix"});

    return true;
}

//从内存主要配置中放置到json
bool MyJson::mainSwitch2json() {
    put<bool>({"main", "relayPrivateMsg"}, relayPrivateMsg);

    put<string>({"main", "prefix"}, prefix);

    return true;
}

//读取单独设置到内存
bool MyJson::json2alone() {
    try {
        alone.clear();

        auto keyList = getKeyList({"alone"});
        for (auto temp_key : keyList) {
            auto& aloneTemp = alone[atoi(temp_key.c_str())];

            //普通关键词
            unordered_set<string> temp_keyword;
            auto& keyWord = aloneTemp.keyWord;
            readJson_array({"alone", temp_key, "keyWord"}, temp_keyword);
            keyWord.clear();
            for (auto temp_keyWord : temp_keyword) {
                keyWord.insert(temp_keyWord);
            }

            //正则表达式关键词
            unordered_set<string> temp_keywordRegex;
            auto& keyWordRegex = aloneTemp.keyWordRegex;
            readJson_array({"alone", temp_key, "keyWordRegex"}, temp_keywordRegex);

            keyWordRegex.clear();
            for (auto temp_keyWordRegex : temp_keywordRegex) {
                keyWordRegex.insert(temp_keyWordRegex);
            }

            //白名单关键词
            unordered_set<string> temp_keywordWhite;
            auto& keyWordWhite = aloneTemp.keyWordWhite;
            readJson_array({"alone", temp_key, "keyWordWhite"}, temp_keywordWhite);
            keyWordWhite.clear();
            for (auto temp_keyWordRegex : temp_keywordWhite) {
                keyWordWhite.insert(temp_keyWordRegex);
            }

            readJson_array({"alone", temp_key, "groupList"}, aloneTemp.groupList);
            readJson_array({"alone", temp_key, "QQList"}, aloneTemp.QQList);
            readJson_array({"alone", temp_key, "relayGroupList"}, aloneTemp.relayGroupList);

            aloneTemp.keyWordGroupWarn = get<bool>({"alone", temp_key, "keyWordGroupWarn"}, false);
            aloneTemp.keyWordPrivateWarn = get<bool>({"alone", temp_key, "keyWordPrivateWarn"}, false);
            aloneTemp.deleteMsg = get<bool>({"alone", temp_key, "deleteMsg"}, false);
            aloneTemp.streng = get<bool>({"alone", temp_key, "streng"}, false);
            aloneTemp.deleteCQCode = get<bool>({"alone", temp_key, "deleteCQCode"}, true);
            aloneTemp.keyWordSendAdmin = get<bool>({"alone", temp_key, "keyWordSendAdmin"}, true);

            aloneTemp.use = get<bool>({"alone", temp_key, "use"}, false);

            aloneTemp.relayGroupMsg_trimFront = get<int>({"alone", temp_key, "relayGroupMsg_trimFront"}, 0);
            aloneTemp.relayGroupMsg_trimBack = get<int>({"alone", temp_key, "relayGroupMsg_trimBack"}, 0);
            aloneTemp.QQListType = get<int>({"alone", temp_key, "QQListType"}, 0);
            aloneTemp.dealType = get<int>({"alone", temp_key, "dealType"}, 0);
            aloneTemp.banTimeLen = get<int>({"alone", temp_key, "banTimeLen"}, 0);
            aloneTemp.priority = get<int>({"alone", temp_key, "priority"}, 0);

            aloneTemp.name = get<string>({"alone", temp_key, "name"}, "");
            aloneTemp.relayGroupWord = get<string>({"alone", temp_key, "relayGroupWord"}, "");
            aloneTemp.keyWordGroupWarnWord = get<string>({"alone", temp_key, "keyWordGroupWarnWord"}, "");
            aloneTemp.keyWordPrivateWarnWord = get<string>({"alone", temp_key, "keyWordPrivateWarnWord"}, "");
        }
    } catch (exception& e) {
        cout << e.what() << endl;
        return false;
    }

    return true;
}

//将单独设置放入json
bool MyJson::alone2json() {
    try {
        conf_json["alone"].clear();

        int index = 1;
        for (auto& temp : alone) {
            string temp_key = "0";
            auto& aloneTemp = temp.second;

            //单独设置
            if (temp.first != 0) temp_key = to_string(index++);

            //普通关键词
            unordered_set<string> temp_keyword;
            for (auto temp_word : aloneTemp.keyWord) {
                temp_keyword.insert(temp_word.keyWord);
            }

            writeJson_array({"alone", temp_key, "keyWord"}, temp_keyword);

            //正则表达式关键词
            unordered_set<string> temp_keyWordRegex;
            for (auto temp_wordRegex : aloneTemp.keyWordRegex) {
                temp_keyWordRegex.insert(temp_wordRegex.keyWord);
            }

            writeJson_array({"alone", temp_key, "keyWordRegex"}, temp_keyWordRegex);

            //白名单关键词
            unordered_set<string> temp_keyWordWhite;
            for (auto temp_wordWhite : aloneTemp.keyWordWhite) {
                temp_keyWordWhite.insert(temp_wordWhite.keyWord);
            }
            writeJson_array({"alone", temp_key, "keyWordWhite"}, temp_keyWordWhite);

            writeJson_array({"alone", temp_key, "groupList"}, aloneTemp.groupList);
            writeJson_array({"alone", temp_key, "QQList"}, aloneTemp.QQList);
            writeJson_array({"alone", temp_key, "relayGroupList"}, aloneTemp.relayGroupList);

            put<bool>({"alone", temp_key, "keyWordPrivateWarn"}, aloneTemp.keyWordPrivateWarn);
            put<bool>({"alone", temp_key, "keyWordGroupWarn"}, aloneTemp.keyWordGroupWarn);
            put<bool>({"alone", temp_key, "deleteMsg"}, aloneTemp.deleteMsg);
            put<bool>({"alone", temp_key, "streng"}, aloneTemp.streng);
            put<bool>({"alone", temp_key, "deleteCQCode"}, aloneTemp.deleteCQCode);
            put<bool>({"alone", temp_key, "keyWordSendAdmin"}, aloneTemp.keyWordSendAdmin);
            put<bool>({"alone", temp_key, "use"}, aloneTemp.use);

            put<int>({"alone", temp_key, "relayGroupMsg_trimFront"}, aloneTemp.relayGroupMsg_trimFront);
            put<int>({"alone", temp_key, "relayGroupMsg_trimBack"}, aloneTemp.relayGroupMsg_trimBack);
            put<int>({"alone", temp_key, "QQListType"}, aloneTemp.QQListType);
            put<int>({"alone", temp_key, "dealType"}, aloneTemp.dealType);
            put<int>({"alone", temp_key, "banTimeLen"}, aloneTemp.banTimeLen);
            put<int>({"alone", temp_key, "priority"}, aloneTemp.priority);

            put<string>({"alone", temp_key, "name"}, aloneTemp.name);
            put<string>({"alone", temp_key, "relayGroupWord"}, aloneTemp.relayGroupWord);
            put<string>({"alone", temp_key, "keyWordGroupWarnWord"}, aloneTemp.keyWordGroupWarnWord);
            put<string>({"alone", temp_key, "keyWordPrivateWarnWord"}, aloneTemp.keyWordPrivateWarnWord);
        }
    } catch (exception& e) {
        cout << e.what() << endl;
        return false;
    }

    return true;
}

//把所有的存放到json中的
void MyJson::all2json() {
    mainSwitch2json();
    admin2json();
    alone2json();
}

//所有的json读取到内存中
void MyJson::json2all() {
    json2mainSwitch();
    json2admin();
    json2alone();
}

//将json写到文件里
void MyJson::json2file() {
    try {
        write_json((appDir + "conf.json"), conf_json);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
}

//将文件的json读取出来
void MyJson::file2json() {
    try {
        read_json((appDir + "conf.json"), conf_json);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
}

//获取 分割每行的字符串 json
void OperateLine::line_get_str(string str, unordered_set<string>& value) {
    value.clear();

    // unordered_set<string> temp_array;
    // boost::split(temp_array, str, boost::is_any_of(L"\r\n"));

    // for (auto temp_str : temp_array) {
    //     //过滤无效值
    //     if (temp_str.empty()) continue;

    //     value.insert(temp_str);
    // }
}

//获取 分割每行的uint json
void OperateLine::line_get_ll(string str, unordered_set<long long>& value) {
    value.clear();

    // unordered_set<string> temp_array;
    // boost::split(temp_array, str, boost::is_any_of(L"\r\n"));

    // for (auto temp_str : temp_array) {
    //     long long num = atoll(temp_str.c_str());

    //     //过滤无效值
    //     if (num == 0) continue;

    //     value.insert(num);
    // }
}
