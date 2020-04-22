#pragma once
#include <string>
#include "appInf.h"

class Update {
public:
    enum updateType {
        no, //没有新版本
        update, //有更新
        mustUpdate, //必须更新
        error, //错误
    };

    updateType check(std::string &inf, int version_id = APP_VERSION_ID);
    bool getUpdate(std::string &inf);

private:
    std::string newVersion;

    std::string appName;
    std::string path;
    std::string host;
    int port;
};