#include "Update.h"
#include "MyUtils.h"
#include "mycq.hpp"

#include <cpr/cpr.h>
#include <json/json.h>

#include <fstream>
#include <string>

using namespace std;

#define URL_UPDATE_CHECK "http://keyword.xiaoxiaoge.cn/api/version"


Update::updateType Update::check(string &inf, int version_id) {
    try {
        auto r = cpr::Get(cpr::Url(URL_UPDATE_CHECK));
        if (r.status_code != 200) {
            throw exception("获取数据失败 网络异常");
        }

        string json = r.text;

        Json::Value root;
        Json::Reader reader;

        reader.parse(json, root);

        int getVersion_id = root["version_id"].asInt();
        if (getVersion_id > version_id) {
            newVersion = root["version"].asString();
            url = root["url"].asString();
            appName = root["file_name"].asString();

            int must = root["must_version_id"].asInt();
            if (must > version_id) {
                inf = "重要更新 " + newVersion + "更新内容:\r\n";
                inf += root["inf"].asString();
                return updateType::mustUpdate;
            }

            inf = "有新版本\r\n" + newVersion + "更新内容:\r\n";
            inf += root["inf"].asString();
            return updateType::update;
        }
        inf = "版本已是最新";
        return updateType::no;

    } catch (exception &e) {
        inf = e.what();
        return updateType::error;
    } catch (...) {
        inf = "未知原因";
        return updateType::error;
    }
}

bool Update::getUpdate(string &inf) {
 
    if (url.empty()) {
        inf = u8"更新链接为空";
        return false;
    }

    try {
        auto r = cpr::Get(cpr::Url(url));
        if (r.status_code != 200) {
            throw exception("插件下载失败 网络异常");
        }
        string appPath = MyUtils::ansi(cq::dir::root()) + "app\\" + MyUtils::ansi(appName);
        ofstream file(appPath, ios::out | ios::binary | ios::trunc);
        if (!file.good()) throw exception(("文件打开失败: " + appPath).c_str());
        file << r.text;
        file.close();
        inf = "更新完成，请重启酷Q载入新版本";
        return true;

    } catch (exception &e) {
        inf = e.what();
        return false;
    } catch (...) {
        inf = "未知原因";
        return false;
    }
}
