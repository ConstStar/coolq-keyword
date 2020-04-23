#include "MyUtils.h"
#include <mycq.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>


wstring MyUtils::string2wstring(string str) {
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
string MyUtils::wstring2string(wstring wstr) {
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

void MyUtils::replace_all(string& str, const string& old_value, const string& new_value) {
    while (true) {
        string::size_type pos(0);
        if ((pos = str.find(old_value)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    // return str;
}

//字符串全部替换
void MyUtils::replace_all_distinct(string& str, string old_value, string new_value) {
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    // return str;
}

string MyUtils::ansi(string& str) {
    return cq::utils::ansi(str);
}

//获取 分割每行的字符串 json
void MyUtils::line_get_str(string str, std::unordered_set<string>& value) {
    value.clear();

    std::unordered_set<string> temp_array;
    boost::split(temp_array, str, boost::is_any_of(L"\r\n"));

    for (auto temp_str : temp_array) {
        //过滤无效值
        if (temp_str.empty()) continue;

        value.insert(temp_str);
    }
}

//获取 分割每行的uint json
void MyUtils::line_get_ll(string str, std::unordered_set<long long>& value) {
    value.clear();

    std::unordered_set<string> temp_array;
    boost::split(temp_array, str, boost::is_any_of(L"\r\n"));

    for (auto temp_str : temp_array) {
        long long num = atoll(temp_str.c_str());

        //过滤无效值
        if (num == 0) continue;

        value.insert(num);
    }
}