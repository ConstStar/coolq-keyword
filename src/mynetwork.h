#pragma once
#include <string>


class Update {
public:
    enum updateType {
        no, //没有新版本
        update, //有更新
        mustUpdate, //必须更新
        error, //错误
    };

    updateType check(int version_id, std::string &inf);
    bool getUpdate(std::string &inf);

private:
    std::string newVersion;

    std::string appName;
    std::string path;
    std::string host;
    int port;
};