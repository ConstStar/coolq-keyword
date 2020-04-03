#include "fun.hpp"
#include "gui.h"

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

    string sendStr = "群关键词监控已开启";
    for (auto one_root : conf.admin) {
        cq::send_private_message(one_root, sendStr);
    }
} catch (exception &e) {
    string sendStr = e.what();
    cq::logging::warning("初始化异常", sendStr.c_str());
    for (auto one_root : conf.admin) {
        cq::send_private_message(one_root, sendStr);
    }
}

CQ_INIT {
    cq::on_enable([] {
        //获取应用数据目录
        appDir = cq::get_app_directory();

        //初始化数据
        init();
        cq::logging::info("群关键词监控", "群监控被启用");
    });

    cq::on_private_message([](const cq::PrivateMessageEvent &event) {
        //try {
            insQQ[event.user_id].put_fromQQ(event.user_id);
            insQQ[event.user_id].funthing(event.message.c_str());
        // } catch (ApiError &e) {


        //     cq::logging::info("错误",e.what());
        // }
    });

    cq::on_group_message([](const cq::GroupMessageEvent &event) {
        //try {
            OperateMsg a(event);
            a.MsgFun();
        // } catch (ApiError &e) {
        //     cq::logging::info("错误",e.what());
        // }
        event.block(); // 阻止当前事件传递到下一个插件
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
