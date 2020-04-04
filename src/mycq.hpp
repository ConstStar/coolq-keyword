#include <cqcppsdk/cqcppsdk.h>
#pragma once
#define APPNAME "群关键词监控"
#define VERSION_ID 17
// namespace mycq::logging {
//     void info(std::string word) {
//         cq::logging::info(APPNAME, word);
//     }

//     void info(std::string title, std::string word) {
//         cq::logging::info(title, word);
//     }

//     void error(std::string word) {
//         cq::logging::error(APPNAME, word);
//     }

//     void error(std::string title, std::string word) {
//         cq::logging::error(title, word);
//     }

//     void warning(std::string word) {
//         cq::logging::warning(APPNAME, word);
//     }

//     void warning(std::string title, std::string word) {
//         cq::logging::warning(title, word);
//     }

//     void test() {
//     }
// } // namespace mycq::logging

class mycq {
public:

    //发送群消息
    static int64_t send_group_message(const int64_t group_id, const std::string &message) {
        try {
            return cq::send_group_message(group_id, message);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }

    //发送私聊消息
    static int64_t send_private_message(const int64_t user_id, const std::string &message) {
        try {
            return cq::send_private_message(user_id, message);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }

    //踢出
    static void set_group_kick(const int64_t group_id, const int64_t user_id, const bool reject_future_request) {
        try {
            cq::set_group_kick(group_id, user_id, reject_future_request);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }

    //撤回消息
    static void delete_message(const int64_t message_id) {
        try {
            cq::delete_message(message_id);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }

    //禁言匿名
    static void set_group_anonymous_ban(const int64_t group_id, const cq::Anonymous &anonymous,
                                        const int64_t duration) {
        try {
            cq::set_group_anonymous_ban(group_id, anonymous, duration);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }

    //禁言
    static void set_group_ban(const int64_t group_id, const int64_t user_id, const int64_t duration) {
        try {
            cq::set_group_ban(group_id, user_id, duration);
        } catch (cq::ApiError &e) {
            cq::logging::info(APPNAME, e.what());
        }
    }
};
// namespace mycq