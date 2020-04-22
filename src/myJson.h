#pragma once
#include <initializer_list>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include <json/json.h>
#include <functional>
#include <iostream>

using namespace std;

//字符串处理
class OperateStr {
public:
    //
    static string& replace_all(string& str, const string& old_value, const string& new_value);

    //字符串全部替换
    static string replace_all_distinct(string& str, string old_value, string new_value);

    //将string转成wstring
    static wstring string2wstring(string str);

    //将wstring转换成string
    static string wstring2string(wstring wstr);
};

struct WKEYWORD {
    WKEYWORD(string& str) : keyWord(str), wkeyWrod(OperateStr::string2wstring(str)) {
    }

    WKEYWORD(const char* str) : keyWord(str), wkeyWrod(OperateStr::string2wstring(string(str))) {
    }

    string keyWord;
    wstring wkeyWrod;

    bool operator==(const WKEYWORD& t) const {
        return hash<string>()(t.keyWord) == hash<string>()(this->keyWord);
    }
};

struct WKEYWORD_hash { //指定hash函数，作为模板的第二个参数
    // hash值为color的hash值
    size_t operator()(const WKEYWORD& _r) const {
        string tmp = _r.keyWord;
        return std::hash<string>()(tmp);
    }
};

//具有单独设置的项目
struct ConfAlone {
    ConfAlone()
        : use(false),
          priority(0),
          name("默认名称"),
          keyWord(),
          keyWordRegex(),
          keyWordWhite(),
          groupList(),
          QQList(),
          relayGroupList(),
          dealType(0),
          banTimeLen(0),
          QQListType(0),
          keyWordGroupWarn(false),
          keyWordPrivateWarn(false),
          streng(false),
          deleteMsg(false),
          deleteCQCode(true),
          keyWordSendAdmin(true),
          relayGroupWord(),
          relayGroupMsg_trimFront(0),
          relayGroupMsg_trimBack(0),
          keyWordGroupWarnWord(),
          keyWordPrivateWarnWord() {
    }

    bool use; //开关
    int priority; //优先级
    string name; //设置名称

    unordered_set<WKEYWORD, WKEYWORD_hash> keyWord; //普通关键词
    unordered_set<WKEYWORD, WKEYWORD_hash> keyWordRegex; //正则表达式关键词
    unordered_set<WKEYWORD, WKEYWORD_hash> keyWordWhite; //白名单关键词
    unordered_set<long long> groupList; //群列表
    unordered_set<long long> QQList; // QQ特殊名单
    unordered_set<long long> relayGroupList; //转发群消息

    int dealType; //处理方法
    int banTimeLen; //禁言时长
    int QQListType; //特殊名单类型  白名单||监控名单
    bool keyWordGroupWarn; //触发后是否群内警告
    bool keyWordPrivateWarn; //触发后是否私聊警告
    bool streng; //强力检测
    bool deleteMsg; //消息撤回
    bool deleteCQCode; //过滤CQ码
    bool keyWordSendAdmin; //提示主人

    string relayGroupWord; //发送到群 消息格式
    int relayGroupMsg_trimFront; //发送到群 消息前几行删除
    int relayGroupMsg_trimBack; //发送到群 消息后几行删除

    string keyWordGroupWarnWord; //触发关键词群内警告
    string keyWordPrivateWarnWord; //触发关键词私聊警告
};

// map排序
struct CmpByKeySize {
    bool operator()(const int& k1, const int& k2) const {
        return k1 < k2;
    }
};

class MyJson {
private:
    //写入json
    bool write_json(string path, Json::Value& root);

    //读取json
    bool read_json(string path, Json::Value& root);

    //获取key列表
    vector<string> getKeyList(const initializer_list<string> json_path);

    //解析json中的数组
    bool readJson_array(const initializer_list<string> json_path, unordered_set<string>& value);

    //解析json中的数组
    bool readJson_array(const initializer_list<string> json_path, unordered_set<long long>& value);

    //构造json中的数组
    bool writeJson_array(const initializer_list<string> json_path, unordered_set<string>& value);

    //构造json中的数组
    bool writeJson_array(const initializer_list<string> json_path, unordered_set<long long>& value);

    //获取json
    template <typename T>
    T get(const initializer_list<string> json_path, T def = T());

    //放置json
    template <typename T>
    void put(const initializer_list<string> json_path, T value);

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
    map<int, ConfAlone, CmpByKeySize> alone; //单独设置
    unordered_set<long long> admin; //主人QQ
    string prefix; //指令消息前缀
    bool relayPrivateMsg; //消息转发

    // Json
    Json::Value conf_json;
};

class OperateLine {
public:
    //获取 分割每行的字符串 json
    static void line_get_str(string str, unordered_set<string>& value);

    //获取 分割每行的uint json
    static void line_get_ll(string str, unordered_set<long long>& value);
};
