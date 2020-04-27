#include "Gui.h"
#include "Update.h"
#include "fun.hpp"

#include <iostream>
#include <set>
#include <sstream>

using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

long long itime = 0;
map<int64_t, Private> insQQ;

void init() try {
    conf.file2json();
    conf.json2all();

    //检查更新
    Update up;
    string inf;
    Update::updateType type = up.check(inf);

    auto sendAdmin = [](string msg) {
        for (auto id : conf.admin) {
            mycq::send_private_message(id, msg);
        }
    };

    //更新
    auto update = [&]() {
        string inf;
        bool ret = up.getUpdate(inf);
        if (ret) {
            string sendStr = "《关键词触发器》\r\n";
            sendStr += inf;
            sendAdmin(sendStr);
        } else {
            string sendStr = "《关键词触发器》\r\n";
            sendStr += inf + "\r\n\r\n有重大更新但更新时出现错误";
            sendAdmin(sendStr);
        }
    };

    //有更新则提示更新消息  无更新则提示插件开启
    if (type == Update::updateType::update) {
        string sendStr = "《关键词触发器》\r\n";
        sendStr += inf + "\r\n\r\n请在界面上下载更新";
        sendAdmin(sendStr);
    } else if (type == Update::updateType::mustUpdate) {
        update();
        string sendStr = "《关键词触发器》\r\n";
        sendStr += inf + "\r\n\r\n更新执行完成，请重启酷Q";
        sendAdmin(sendStr);
    } else {
        string sendStr = "《关键词触发器》已开启";
        sendAdmin(sendStr);
    }

    if (conf.admin.empty()) {
        cq::logging::info("关键词触发器", "未设置主人，建议在界面设置主人QQ");
    }

} catch (exception &e) {
    string sendStr = "《关键词触发器》初始化异常: ";
    sendStr += e.what();
    cq::logging::warning("关键词触发器", sendStr);
    for (auto one_root : conf.admin) {
        mycq::send_private_message(one_root, sendStr + "\r\n请尝试重启酷Q");
    }
}

CQ_INIT {
    cq::on_enable([] {
        //获取应用数据目录
        appDir = cq::get_app_directory();

        //初始化数据
        init();
        cq::logging::info("关键词触发器", "关键词触发器已被启用");
    });

    cq::on_private_message([](const cq::PrivateMessageEvent &event) {
        // try {
        insQQ[event.user_id].init(event.user_id);
        insQQ[event.user_id].funthing(cq::message::unescape(event.message).c_str());

        // } catch (ApiError &e) {

        //     cq::logging::info("错误",e.what());
        // }
    });

    cq::on_group_message([](const cq::GroupMessageEvent &event) {
        // try {
        GroupKeyWord a(event);
        a.MsgFun();
        // } catch (ApiError &e) {
        //     cq::logging::info("错误",e.what());
        // }
    });
}

CQ_MENU(menu) {
    try {
        Gui gui;
        gui.openMain();

    } catch (exception &e) {
        MessageBoxA(NULL, e.what(), "界面异常崩溃", MB_OK);
    } catch (...) {
        MessageBoxA(NULL, "未知原因", "界面异常崩溃", MB_OK);
    }
}
