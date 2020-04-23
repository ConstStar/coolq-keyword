#include <string>
#include <vector>
#include <unordered_set>

using string = std::string;
using wstring = std::wstring;

//字符串处理
class MyUtils {
public:
    //
    static void replace_all(string& str, const string& old_value, const string& new_value);

    //字符串全部替换
    static void replace_all_distinct(string& str, string old_value, string new_value);

    //将string转成wstring
    static wstring string2wstring(string str);

    //将wstring转换成string
    static string wstring2string(wstring wstr);

    static string ansi(string& str);

    //获取 分割每行的字符串 json
    static std::unordered_set<string> line_get_str(string str);

    //获取 分割每行的uint json
    static std::unordered_set<long long> line_get_ll(string str);

    //字符串分割函数
    static std::vector<std::string> MyUtils::split(std::string& str, std::string pattern);

    //删除所有指定字符
    static void string_erase_all(std::string& str, char c);
};