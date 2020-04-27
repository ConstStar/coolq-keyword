#pragma once
#include <cpr/cpr.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

class moderation {
protected:
    bool read_json(string buf, Json::Value& root) {
        stringstream io_json;
        io_json << buf;

        Json::CharReaderBuilder rbuilder;
        rbuilder["collectComments"] = false;
        std::string errs;
        bool ok = Json::parseFromStream(rbuilder, io_json, &root, &errs);

        if (!ok || !errs.empty()) return false;

        return true;
    }

public:
    virtual void init(const string& userName, const string& domainName, const string& passWord,
                      const string& projectName) {
    }

    virtual int text(const string& msg, string& inf) {
        cout << "error";
        return -1;
    }
};

class moderationHuaWei : public moderation {
private:
    string getToken() {
        m_updataTime = time(NULL);
        string body =
            "{\"auth\":{\"identity\":{\"methods\":[\"password\"],\"password\":{\"user\":{\"domain\":{\"name\":\"";
        body += m_domainName;
        body += "\"},\"name\":\"";
        body += m_userName;
        body += "\",\"password\":\"";
        body += m_passWord;
        body += "\"}}},\"scope\":{\"project\":{\"name\":\"";
        body += m_projectName;
        body += "\"}}}}";

        auto r = cpr::Post(cpr::Url{"https://iam.myhuaweicloud.com/v3/auth/tokens"}, cpr::Body{body});
        // cout << r.text << endl;
        return r.header["x-subject-token"];
    }

public:
    void init(const string& userName, const string& domainName, const string& passWord, const string& projectName) {
        m_userName = userName;
        m_domainName = domainName;
        m_passWord = passWord;
        m_projectName = projectName;
        m_updataTime = 0;
    }

    int text(const string& msg, string& inf) {
        try {
            string body =
                u8"{\"categories\":[\"politics\",\"porn\",\"ad\",\"abuse\",\"contraband\"],\"items\":[{\"text\": \""
                + msg + "\"}]}";
            if (time(NULL) - m_updataTime > 23 * 60) {
                m_token = getToken();

                // Token 为空
                if (m_token.empty()) {
                    m_updataTime = 0;
                    throw exception("获取Token失败，请检查填写信息是否正确");
                }
            }

            auto r =
                cpr::Post(cpr::Url{"https://moderation." + m_projectName + ".myhuaweicloud.com/v1.0/moderation/text"},
                          cpr::Body{body},
                          cpr::Header{{"Content-Type", "application/json"}, {"X-Auth-Token", m_token}});

            Json::Value root;
            read_json(r.text, root);

            if (!root["error_msg"].isNull()) {
                throw exception(root["error_msg"].asCString());
            }

            string suggestion = root["result"]["suggestion"].asString();
            if (suggestion == "pass") {
                inf = "检测通过";
                return 0;
            } else if (suggestion == "block") {
                map<string, string> keywordList;

                map<string, string> keywordType;
                keywordType["politics"] = u8"涉政";
                keywordType["porn"] = u8"涉黄";
                keywordType["ad"] = u8"广告";
                keywordType["abuse"] = u8"辱骂";
                keywordType["contraband"] = u8"违禁品";

                auto& detail = root["result"]["detail"];
                for (auto& key : detail.getMemberNames()) {
                    for (auto keyword : detail[key]) {
                        if (keywordList[key].empty())
                            keywordList[key] += "[";
                        else
                            keywordList[key] += ",";

                        keywordList[key] += keyword.asString();
                    }
                    keywordList[key] += "]";
                }

                inf.erase();
                for (auto& keyword : keywordList) {
                    if (!inf.empty()) inf += ",";

                    inf += keywordType[keyword.first];
                    inf += ":";
                    inf += keyword.second;
                }

                return 1;
            } else if (suggestion == "review") {
                inf = "需要人工复查";
                return 0;
            }

        } catch (exception& e) {
            inf = "华为云：";
            inf += e.what();
            return -1;
        }

        return -1;
    }

private:
    string m_token;

    time_t m_updataTime;
    string m_userName;
    string m_domainName;
    string m_passWord;
    string m_projectName;
};