#include "gui.h"
#include "myJson.h"
#include "mycq.hpp"
#include "mynetwork.h"

#include <regex>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>

using namespace nana;
using namespace std;

extern MyJson conf;

//主要设置
class tab_page_main : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        //主人QQ
        text_admin.reset();
        for (auto id : conf.admin) {
            text_admin.append(to_string(id) + "\n", true);
        }

        //指令前缀
        text_prefix.reset(conf.prefix);

        //私聊消息转发给主人
        check_relayPrivateMsg.check(conf.relayPrivateMsg);
    }

    //初始化
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            //主人QQ
            "<vert <weight=25 lab_admin> <text_admin>>"
            "<weight=10>"

            "<vert"
            "<weight=25>"

            //指令前缀
            "<weight=25 <weight=65 lab_prefix><text_prefix>>"
            "<weight=10>"

            //开关
            "<check>"
            "<>"

            //使用教程 //在线更新 //反馈
            "<weight=60 <button_document> <button_update> <button_feedback>>"
            "<>"

            // //绑定秘钥
            // "<weight=25 lab_usingKey>"
            // "<weight=25 <text_usingKey><weight=60 button_usingKey>>"
            // "<>"

            //保存
            "<weight=30% button_save>"

            ">");

        //主人QQ
        lab_admin.create(*this);
        lab_admin.caption(u8"主人QQ(每行一个):");
        place_.field("lab_admin") << lab_admin;

        text_admin.create(*this);
        place_.field("text_admin") << text_admin;

        //指令前缀
        lab_prefix.create(*this);
        lab_prefix.caption(u8"指令前缀：");
        place_.field("lab_prefix") << lab_prefix;

        //收到私聊消息转发给主人
        check_relayPrivateMsg.create(*this);
        check_relayPrivateMsg.caption(u8"收到的私聊消息转发给主人");
        place_.field("check") << check_relayPrivateMsg;

        text_prefix.create(*this);
        text_prefix.line_wrapped(false);
        text_prefix.multi_lines(false);
        place_.field("text_prefix") << text_prefix;

        button_document.create(*this);
        button_document.caption(u8"使用教程");
        button_document.events().click([this] {
            msgbox m_inf{*this, u8"提示"};
            m_inf << u8"待开发";
            m_inf.show();
        });
        place_.field("button_document") << button_document;

        button_update.create(*this);
        button_update.caption(u8"检查更新");
        button_update.events().click([this] {
            Update up;
            string inf;
            Update::updateType type = up.check(VERSION_ID, inf);

            //更新
            auto update = [&]() {
                string inf;
                bool ret = up.getUpdate(inf);
                if (ret) {
                    msgbox m_inf{*this, u8"更新完成"};
                    m_inf << inf;
                    m_inf.show();
                } else {
                    msgbox m_error{*this, u8"更新错误"};
                    m_error.icon(msgbox::icon_error);
                    m_error << inf;
                    m_error.show();
                }
            };

            if (type == Update::updateType::no) {
                msgbox m_inf{*this, u8"无需更新"};
                m_inf << inf;
                m_inf.show();
            } else if (type == Update::updateType::update) {
                msgbox m_inf{*this, u8"有新版本", msgbox::yes_no};
                m_inf << inf << "\n\n";
                m_inf << "是否更新";
                auto ret = m_inf.show();
                if (ret == msgbox::pick_yes) {
                    update();
                }
            } else if (type == Update::updateType::mustUpdate) {
                update();
                msgbox m_inf{*this, u8"更新内容"};
                m_inf << inf;
                m_inf.show();
            } else if (type == Update::updateType::error) {
                msgbox m_error{*this, u8"错误"};
                m_error.icon(msgbox::icon_error);
                m_error << inf;
                m_error.show();
            }
        });
        place_.field("button_update") << button_update;

        button_feedback.create(*this);
        button_feedback.caption(u8"问题反馈");
        button_feedback.events().click([this] {
            msgbox m_inf{*this, u8"提示"};
            m_inf << u8"待开发";
            m_inf.show();
        });
        place_.field("button_feedback") << button_feedback;

        // //使用秘钥
        // lab_usingKey.create(*this);
        // lab_usingKey.caption(u8"激活专业版：");
        // place_.field("lab_usingKey") << lab_usingKey;

        // text_usingKey.create(*this);
        // text_usingKey.line_wrapped(false);
        // text_usingKey.multi_lines(false);
        // place_.field("text_usingKey") << text_usingKey;

        // btn_usingKey.create(*this);
        // btn_usingKey.caption(u8"使用秘钥");
        // btn_usingKey.events().click([this] {

        // });
        // place_.field("button_usingKey") << btn_usingKey;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            bool ok = regex_match(text_admin.text(), regex("[\\n\\d\\r]*"));
            if (!ok) {
                msgbox m_error{*this, u8"错误"};
                m_error.icon(msgbox::icon_error);
                m_error << u8"主人QQ格式有误";
                m_error.show();
                return;
            }

            //主人QQ
            auto line = text_admin.text_line_count();
            conf.admin.clear();
            for (int i = 0; i < line; i++) {
                string buf;
                text_admin.getline(i, buf);
                if (!buf.empty()) conf.admin.push_back(atoll(buf.c_str()));
            }

            //消息前缀
            conf.prefix = text_prefix.text();

            //收到的私聊消息转发给主人
            conf.relayPrivateMsg = check_relayPrivateMsg.checked();

            //写入配置
            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            //重新载入配置
            readConf();
        });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_main(window wd) : panel<false>(wd) {
        init();
    }

private:
    place place_;
    int conf_index;

    //主人QQ
    label lab_admin;
    textbox text_admin;

    //指令前缀
    label lab_prefix;
    textbox text_prefix;

    //收到的私聊消息转发主人
    checkbox check_relayPrivateMsg;

    // //使用秘钥
    // label lab_usingKey;
    // textbox text_usingKey;
    // button btn_usingKey;

    button button_document;
    button button_update;
    button button_feedback;

    //保存
    button btn_save;
};

//单独设置中的主要设置
class tab_page_aloneMain : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        text_name.reset(conf.alone[conf_index].name);
        text_priority.from(conf.alone[conf_index].priority);
    }

    //初始化
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            "<vert"
            "<weight=25>"

            //优先级
            "<weight=25 <weight=65 lab_priority><weight=65 text_priority>>"
            "<weight=10>"

            //设置名称
            "<weight=25 <weight=65 lab_name><weight=65 text_name>>"
            "<weight=60%>"

            //保存
            "<<> <button_save> <>>"

            ">");

        //设置名称
        lab_priority.create(*this);
        lab_priority.caption(u8"优先级：");
        place_.field("lab_priority") << lab_priority;

        text_priority.create(*this);
        text_priority.line_wrapped(false);
        text_priority.multi_lines(false);
        place_.field("text_priority") << text_priority;

        //设置名称
        lab_name.create(*this);
        lab_name.caption(u8"设置名称：");
        place_.field("lab_name") << lab_name;

        text_name.create(*this);
        text_name.line_wrapped(false);
        text_name.multi_lines(false);
        place_.field("text_name") << text_name;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            int priority_temp = text_priority.to_int();
            // if (priority_temp <= 0) {
            //     msgbox m_error{*this, u8"错误"};
            //     m_error.icon(msgbox::icon_error);
            //     m_error << u8"优先级必须为正数";
            //     m_error.show();

            //     return;
            // }

            conf.alone[conf_index].priority = priority_temp;
            conf.alone[conf_index].name = text_name.text();

            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            //重新载入配置
            readConf();
        });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_aloneMain(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //优先级
    label lab_priority;
    textbox text_priority;

    //设置名称
    label lab_name;
    textbox text_name;

    //保存
    button btn_save;
};

//全局设置
class tab_page_overall : public panel<false> {
private:
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    void readConf() {
        conf.file2json();
        conf.json2all();

        //载入群名单
        auto groupList = cq::get_group_list();

        //清空原有
        listGroupList.erase();

        auto& conf_groupList = conf.alone[conf_index].groupList;
        //先载入已选
        for (auto id : conf_groupList) {
            string groupName;
            for (auto& group : groupList) {
                if (group.group_id == id) {
                    groupName = group.group_name;
                    break;
                }
            }

            listGroupList.at(0).append({groupName, to_string(id)});
        }

        //再载入未选
        for (auto temp : groupList) {
            if (find(conf_groupList.begin(), conf_groupList.end(), temp.group_id) == conf_groupList.end())
                listGroupList.at(0).append({temp.group_name, to_string(temp.group_id)});
        }

        //勾选已选的
        auto size = listGroupList.size_item(0);
        for (int i = 0; i < size; i++) {
            string buf = listGroupList.at(0).at(i).text(1);

            if (find(conf.alone[conf_index].groupList.begin(),
                     conf.alone[conf_index].groupList.end(),
                     atoll(buf.c_str()))
                != conf.alone[conf_index].groupList.end()) {
                listGroupList.at(0).at(i).check(true);
            }
        }

        //处理方式
        checkDeal.at(conf.alone[conf_index].dealType)->check(true);

        //禁言时长
        textBenTimeLen.reset(to_string(conf.alone[conf_index].banTimeLen));

        //一些开关
        checkDeleteMsg.check(conf.alone[conf_index].deleteMsg);
        checkStreng.check(conf.alone[conf_index].streng);
        check_deleteCQCode.check(conf.alone[conf_index].deleteCQCode);
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            //群名单
            "<vert weight=250 <list> <weight=25 <text_addgroup><weight=80 button_addgroup>><weight=5><weight=30 "
            "gap=[10,10] arrange=[40,40] button_list>>"
            "<weight=20>"

            "<vert"
            //处理方式
            "<weight=20 labelDeal>"
            "<vert weight=25% checkDeal>"
            "<weight=20 <weight=25% labelBanTimeLen> <weight=25% TextBanTimeLen><labelBanTimeLenTip>>"
            "<weight=10>"

            //一些开关
            "<weight=25 labelSwitch>"
            "<weight=20% vert checkSwitch>"
            "<weight=10>"

            //保存按钮
            "<weight=10% btnSave>"

            ">");

        //列表
        listGroupList.create(*this);
        listGroupList.checkable(true);
        listGroupList.append_header(u8"群名");
        listGroupList.append_header(u8"群号码");
        place_.field("list") << listGroupList;

        //手动添加群
        textAddGroup.create(*this);
        textAddGroup.line_wrapped(false);
        textAddGroup.multi_lines(false);
        place_.field("text_addgroup") << textAddGroup;

        btnGroupAdd.create(*this);
        btnGroupAdd.caption(u8"手动添加");
        btnGroupAdd.events().click([this] {
            bool ok = regex_match(textAddGroup.text(), regex("[1-9][0-9]*"));
            if (!ok) {
                msgbox m_error{*this, u8"错误"};
                m_error.icon(msgbox::icon_error);
                m_error << u8"格式有误";
                m_error.show();
                return;
            }

            listGroupList.at(0).append({"", textAddGroup.text()});

            //查找添加项并勾选
            auto size = listGroupList.size_item(0);
            for (int i = 0; i < size; i++) {
                string buf = listGroupList.at(0).at(i).text(1);

                if (buf == textAddGroup.text()) {
                    listGroupList.at(0).at(i).check(true);
                    break;
                }
            }
        });
        place_.field("button_addgroup") << btnGroupAdd;

        //列表按钮
        //全选
        btnListAll.create(*this);
        btnListAll.caption(u8"全选");
        btnListAll.events().click([this] {
            auto size = listGroupList.size_item(0);

            for (int i = 0; i < size; i++) {
                listGroupList.at(listbox::index_pair(0, i)).check(true);
            }
        });
        //反选
        btnListReverse.create(*this);
        btnListReverse.caption(u8"反选");
        btnListReverse.events().click([this] {
            auto size = listGroupList.size_item(0);

            for (int i = 0; i < size; i++) {
                auto p = listGroupList.at(listbox::index_pair(0, i));

                p.check(!p.checked());
            }
        });
        place_.field("button_list") << btnListAll << btnListReverse;

        //处理方式
        labelDeal.create(*this);
        labelDeal.caption(u8"处理方式:");
        place_.field("labelDeal") << labelDeal;

        vector<string> groupStr;
        groupStr.push_back(u8"不做处理");
        groupStr.push_back(u8"禁言");
        groupStr.push_back(u8"踢出");
        groupStr.push_back(u8"拉黑并踢出");
        for (int i = 0; i < groupStr.size(); i++) {
            auto p = std::make_shared<checkbox>(*this);
            checkDeal.push_back(p);

            // Add the checkbox to the radio group. The radio group does not
            // manage the life of checkboxs.
            groupDeal.add(*p);
            place_.field("checkDeal") << *p;

            p->caption(groupStr[i]);
            p->events().click([this]() {
                std::size_t index = this->groupDeal.checked();
                std::string str = this->checkDeal[index]->caption();

                if (str == u8"禁言") {
                    labelBanTimeLenTip.caption(u8"分钟");
                } else {
                    labelBanTimeLenTip.caption(u8"分钟(禁言方式下生效)");
                }
            });
        }

        labelBenTimeLen.create(*this);
        labelBenTimeLen.caption(u8"禁言时长:");
        place_.field("labelBanTimeLen") << labelBenTimeLen;

        textBenTimeLen.create(*this);
        textBenTimeLen.line_wrapped(false);
        textBenTimeLen.multi_lines(false);
        place_.field("TextBanTimeLen") << textBenTimeLen;

        labelBanTimeLenTip.create(*this);
        labelBanTimeLenTip.caption(u8"分钟");
        place_.field("labelBanTimeLenTip") << labelBanTimeLenTip;

        //一些功能开关
        labelSwitch.create(*this);
        labelSwitch.caption(u8"一些功能开关:");
        place_.field("labelSwitch") << labelSwitch;

        checkStreng.create(*this);
        checkStreng.caption(u8"关键词强力检测");
        place_.field("checkSwitch") << checkStreng;

        checkDeleteMsg.create(*this);
        checkDeleteMsg.caption(u8"撤回触发关键词消息");
        place_.field("checkSwitch") << checkDeleteMsg;

        check_deleteCQCode.create(*this);
        check_deleteCQCode.caption(u8"过滤CQ码（如图片消息，签到消息等）");
        place_.field("checkSwitch") << check_deleteCQCode;

        btnSave.create(*this);
        btnSave.caption(u8"保存");
        btnSave.events().click([this] {
            //群名单
            conf.alone[conf_index].groupList.clear();
            for (auto temp : listGroupList.checked()) {
                string buf = listGroupList.at(temp).text(1);

                conf.alone[conf_index].groupList.push_back(atoll(buf.c_str()));
            }

            //处理方式
            int dealType = 0;
            for (int i = 0; i < checkDeal.size(); i++) {
                if (checkDeal.at(i)->checked()) {
                    dealType = i;
                    break;
                }
            }
            conf.alone[conf_index].dealType = dealType;

            //禁言时长
            conf.alone[conf_index].banTimeLen = atoll(textBenTimeLen.text().c_str());

            //一些开关
            conf.alone[conf_index].deleteMsg = checkDeleteMsg.checked();
            conf.alone[conf_index].streng = checkStreng.checked();
            conf.alone[conf_index].deleteCQCode = check_deleteCQCode.checked();

            //写入配置
            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            //重新载入配置
            readConf();
        });
        place_.field("btnSave") << btnSave;

        //读取配置
        readConf();
    }

public:
    tab_page_overall(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //群名单
    listbox listGroupList; //群列表

    textbox textAddGroup; //手动添加群	文本框
    button btnGroupAdd; //手动添加群	按钮

    button btnListReverse; //反选
    button btnListAll; //全选

    //处理方式
    label labelDeal;
    std::vector<std::shared_ptr<checkbox>> checkDeal;
    radio_group groupDeal;
    label labelBenTimeLen; //禁言时间 标签
    label labelBanTimeLenTip; //禁言时间 提示标签
    textbox textBenTimeLen; //禁言时间 文本框

    //一些功能开关
    label labelSwitch;
    checkbox checkStreng; //强力检测
    checkbox checkDeleteMsg; //撤回消息
    checkbox check_deleteCQCode; //过滤CQ码

    //保存
    button btnSave;
};

//列表
class tab_page_list : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        // QQ列表
        text_QQList.reset();
        for (auto id : conf.alone[conf_index].QQList) {
            text_QQList.append(to_string(id) + "\n", true);
        }

        //白名单关键词
        text_keyWordWhite.reset();
        for (auto keyword : conf.alone[conf_index].keyWordWhite) {
            text_keyWordWhite.append(keyword.keyWord + "\n", true);
        }

        //特殊名单类型
        check_QQListType.at(conf.alone[conf_index].QQListType)->check(true);
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            //左边
            "<vert"

            //白名单关键词
            "<vert <weight=25 lab_keyWordWhite> <text_keyWordWhite>>"
            "<weight=10>"

            //设置特殊名单为  白名单 || 监控名单
            "<vert weight=30% <eight=30% lab_QQListType> <eight=30% check_QQListType> <lab_QQListTypeTip>>"

            ">"

            "<weight=10>"

            //右边
            "<vert"

            //特殊QQ名单
            "<vert <weight=25 <lab_QQList>> <text_QQList>>"
            "<weight=10>"

            //保存
            "<weight=25% button_save>"

            ">");

        //白名单关键词
        lab_keyWordWhite.create(*this);
        lab_keyWordWhite.caption(u8"白名单关键词(每行一个):");
        place_.field("lab_keyWordWhite") << lab_keyWordWhite;

        text_keyWordWhite.create(*this);
        text_keyWordWhite.line_wrapped(true);
        text_keyWordWhite.tip_string(u8"包含这些关键词的消息将不会检测");
        place_.field("text_keyWordWhite") << text_keyWordWhite;

        //特殊QQ名单
        lab_QQList.create(*this);
        lab_QQList.caption(u8"QQ 白名单/监控名单(每行一个):");
        place_.field("lab_QQList") << lab_QQList;

        text_QQList.create(*this);
        place_.field("text_QQList") << text_QQList;

        //设置特殊名单为  白名单 || 监控名单
        lab_QQListType.create(*this);
        lab_QQListType.caption(u8"设置名单类型为:");
        place_.field("lab_QQListType") << lab_QQListType;

        lab_QQListTypeTip.create(*this);
        place_.field("lab_QQListTypeTip") << lab_QQListTypeTip;

        vector<string> groupStr;
        groupStr.push_back(u8"白名单");
        groupStr.push_back(u8"监控名单");

        for (int i = 0; i < groupStr.size(); i++) {
            auto p = std::make_shared<checkbox>(*this);
            check_QQListType.push_back(p);

            group_QQListType.add(*p);
            place_.field("check_QQListType") << *p;

            p->caption(groupStr[i]);
            p->events().click([this]() {
                std::size_t index = this->group_QQListType.checked();
                std::string str = this->check_QQListType[index]->caption();

                if (str == u8"白名单") {
                    lab_QQListTypeTip.caption(u8"当前为白名单模式，不会监控名单中的QQ");
                    lab_QQList.caption(u8"QQ白名单(每行一个):");
                } else if (str == u8"监控名单") {
                    lab_QQListTypeTip.caption(u8"当前为监控名单模式，只监控名单中的QQ");
                    lab_QQList.caption(u8"QQ监控名单(每行一个):");
                } else {
                    lab_QQListTypeTip.caption("");
                }
            });
        }

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            // QQ特殊名单
            auto QQList_line = text_QQList.text_line_count();
            conf.alone[conf_index].QQList.clear();
            for (int i = 0; i < QQList_line; i++) {
                string buf;
                text_QQList.getline(i, buf);
                if (!buf.empty()) conf.alone[conf_index].QQList.push_back(atoll(buf.c_str()));
            }

            //白名单关键词
            auto keyWrodWhite_line = text_keyWordWhite.text_line_count();
            conf.alone[conf_index].keyWordWhite.clear();
            for (int i = 0; i < keyWrodWhite_line; i++) {
                string buf;
                text_keyWordWhite.getline(i, buf);
                if (!buf.empty()) conf.alone[conf_index].keyWordWhite.push_back(buf.c_str());
            }

            //特殊名单类型  白名单||监控名单
            int listType = 0;
            for (int i = 0; i < check_QQListType.size(); i++) {
                if (check_QQListType.at(i)->checked()) {
                    listType = i;
                    break;
                }
            }
            conf.alone[conf_index].QQListType = listType;

            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            //重新读取配置
            readConf();
        });

        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_list(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //白名单关键词
    label lab_keyWordWhite;
    textbox text_keyWordWhite;

    // QQ列表
    label lab_QQList;
    textbox text_QQList;

    //特殊QQ名单类型
    label lab_QQListType;
    std::vector<std::shared_ptr<checkbox>> check_QQListType;
    radio_group group_QQListType;
    label lab_QQListTypeTip;

    //保存
    button btn_save;
};

//关键词
class tab_page_keyWord : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        //普通关键词
        text_keyWord.reset();
        for (auto keyword : conf.alone[conf_index].keyWord) {
            text_keyWord.append(keyword.keyWord + "\n", true);
        }

        //正则表达式关键词
        text_keyWordRegex.reset();
        for (auto keyword : conf.alone[conf_index].keyWordRegex) {
            text_keyWordRegex.append(keyword.keyWord + "\n", true);
        }
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            //普通关键词
            "<vert <weight=25 lab_keyWord> <text_keyWord>>"
            "<weight=10>"

            "<vert"

            //正则表达式关键词
            "<vert <weight=25 <lab_keyWordRegex> <weight=30% margin = [0,0,7] btn_regexTest>> <text_keyWordRegex>>"
            "<weight=10>"

            //保存
            "<weight=25% button_save>"

            ">");

        //普通关键词
        lab_keyWord.create(*this);
        lab_keyWord.caption(u8"普通关键词(每行一个):");
        place_.field("lab_keyWord") << lab_keyWord;

        text_keyWord.create(*this);
        text_keyWord.line_wrapped(true);
        place_.field("text_keyWord") << text_keyWord;

        //正则表达式关键词
        lab_keyWordRegex.create(*this);
        lab_keyWordRegex.caption(u8"正则表达式关键词(每行一个):");
        place_.field("lab_keyWordRegex") << lab_keyWordRegex;

        text_keyWordRegex.create(*this);
        text_keyWordRegex.line_wrapped(true);
        place_.field("text_keyWordRegex") << text_keyWordRegex;

        //正则表达式测试
        btn_regexTest.create(*this);
        btn_regexTest.caption(u8"测试正则");
        btn_regexTest.events().click([this] { mynetwork::openUrl("https://c.runoob.com/front-end/854"); });
        place_.field("btn_regexTest") << btn_regexTest;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            //关键词
            auto keyWord_line = text_keyWord.text_line_count();
            conf.alone[conf_index].keyWord.clear();
            for (int i = 0; i < keyWord_line; i++) {
                string buf;
                text_keyWord.getline(i, buf);
                if (!buf.empty()) conf.alone[conf_index].keyWord.push_back(buf.c_str());
            }

            //正则表达式关键词
            auto keyWordRegex_line = text_keyWordRegex.text_line_count();
            conf.alone[conf_index].keyWordRegex.clear();
            for (int i = 0; i < keyWordRegex_line; i++) {
                string buf;
                text_keyWordRegex.getline(i, buf);
                if (!buf.empty()) conf.alone[conf_index].keyWordRegex.push_back(buf.c_str());
            }

            //写入配置
            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            //重新读取配置
            readConf();
        });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_keyWord(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //主人QQ
    label lab_keyWord;
    textbox text_keyWord;

    //正则表达式关键词
    label lab_keyWordRegex;
    textbox text_keyWordRegex;

    //正则表达式测试
    button btn_regexTest;

    //保存
    button btn_save;
};

//自定义触发回复
class tab_page_groupWarnWord : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        //触发后回复群消息
        check_groupWarn.check(conf.alone[conf_index].keyWordGroupWarn);
        text_groupWarn.reset(conf.alone[conf_index].keyWordGroupWarnWord);

        //触发后回复私聊消息
        check_privateWarn.check(conf.alone[conf_index].keyWordPrivateWarn);
        text_privateWarn.reset(conf.alone[conf_index].keyWordPrivateWarnWord);
    }

    //显示文本
    void showText(string title, string word) {
        form fm_temp;
        fm_temp.caption(title);

        textbox text{fm_temp};
        text.caption(word);
        text.editable(false);

        fm_temp.div("<text>");
        fm_temp["text"] << text;
        fm_temp.collocate();

        fm_temp.show();
        exec();
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            //触发后回复群消息
            "<vert <weight=30 check_groupWarn> <weight=25 <lab_groupWarn> <weight = 40% margin = [0,0,7]  "
            "button_groupVariable>> "
            "<text_groupWarn>>"
            "<weight=10>"

            "<vert"

            //触发后回复私聊消息
            "<vert <weight=30 check_privateWarn> <weight=25 <lab_privateWarn> <weight = 40% margin = [0,0,7]  "
            "button_privateVariable>> "
            "<text_privateWarn>>"
            "<weight=10>"

            //保存
            "<weight=25% button_save>"

            ">");

        //触发回复群消息开关
        check_groupWarn.create(*this);
        check_groupWarn.caption(u8"触发关键词发送群消息提醒");
        place_.field("check_groupWarn") << check_groupWarn;

        //触发后回复私聊消息开关
        check_privateWarn.create(*this);
        check_privateWarn.create(*this);
        check_privateWarn.caption(u8"触发关键词发送私聊消息提醒");
        place_.field("check_privateWarn") << check_privateWarn;

        //触发后回复群消息内容
        lab_groupWarn.create(*this);
        lab_groupWarn.caption(u8"触发后回复群消息内容:");
        place_.field("lab_groupWarn") << lab_groupWarn;

        text_groupWarn.create(*this);
        text_groupWarn.line_wrapped(true);
        place_.field("text_groupWarn") << text_groupWarn;

        //触发后发送私聊消息
        lab_privateWarn.create(*this);
        lab_privateWarn.caption(u8"触发后回复私聊消息内容:");
        place_.field("lab_privateWarn") << lab_privateWarn;

        text_privateWarn.create(*this);
        text_privateWarn.line_wrapped(true);
        place_.field("text_privateWarn") << text_privateWarn;

        //变量按钮
        button_groupVariable.create(*this);
        button_groupVariable.caption(u8"可用变量");
        button_groupVariable.events().click([this] {
            string variable(
                u8"{at}\t\t艾特\r\n"
                u8"{日期}\t\t当前日期\r\n"
                u8"{时间}\t\t当前时间\r\n"
                u8"{星期}\t\t当前星期\r\n"
                u8"{处理方式}\t触发后处理方式\r\n"
                u8"{关键词}\t\t触发的关键词\r\n"
                u8"{QQ号码}\t触发关键词的QQ号码\r\n"
                u8"{QQ名称}\t触发关键词的QQ名称\r\n"
                u8"{QQ名片}\t触发关键词的QQ名片\r\n"
                u8"{群号码}\t触发关键词的群名称\r\n"
                u8"{群名称}\t触发关键词的群名称\r\n"
                u8"\r\n更多变量欢迎进群补充(群:839067703)");
            showText("回复内容变量", variable);
        });
        place_.field("button_groupVariable") << button_groupVariable;

        button_privateVariable.create(*this);
        button_privateVariable.caption(u8"可用变量");
        button_privateVariable.events().click([this] {
            string variable(
                u8"{at}\t\t艾特\r\n"
                u8"{日期}\t\t当前日期\r\n"
                u8"{时间}\t\t当前时间\r\n"
                u8"{星期}\t\t当前星期\r\n"
                u8"{处理方式}\t触发后处理方式\r\n"
                u8"{关键词}\t\t触发的关键词\r\n"
                u8"{QQ号码}\t触发关键词的QQ号码\r\n"
                u8"{QQ名称}\t触发关键词的QQ名称\r\n"
                u8"{QQ名片}\t触发关键词的QQ名片\r\n"
                u8"{群号码}\t触发关键词的群名称\r\n"
                u8"{群名称}\t触发关键词的群名称\r\n"
                u8"\r\n更多变量欢迎进群补充(群:839067703)");
            showText("回复内容变量", variable);
        });
        place_.field("button_privateVariable") << button_privateVariable;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            conf.alone[conf_index].keyWordGroupWarnWord = text_groupWarn.text();
            conf.alone[conf_index].keyWordGroupWarn = check_groupWarn.checked();

            conf.alone[conf_index].keyWordPrivateWarnWord = text_privateWarn.text();
            conf.alone[conf_index].keyWordPrivateWarn = check_privateWarn.checked();

            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            readConf();
        });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_groupWarnWord(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //触发后回复群消息开关
    checkbox check_groupWarn;

    //触发后回复私聊消息开关
    checkbox check_privateWarn;

    //触发后回复群消息内容
    label lab_groupWarn;
    textbox text_groupWarn;

    //触发后发送私聊消息
    label lab_privateWarn;
    textbox text_privateWarn;

    //变量
    button button_groupVariable;
    button button_privateVariable;

    //保存
    button btn_save;
};

//转发到群
class tab_page_relayGroupMsg : public panel<false> {
private:
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    void readConf() {
        conf.file2json();
        conf.json2all();

        //载入群名单
        auto groupList = cq::get_group_list();

        //清空原有
        list_groupList.erase();

        auto& conf_groupList = conf.alone[conf_index].relayGroupList;
        //先载入已选
        for (auto id : conf_groupList) {
            string groupName;
            for (auto& group : groupList) {
                if (group.group_id == id) {
                    groupName = group.group_name;
                    break;
                }
            }

            list_groupList.at(0).append({groupName, to_string(id)});
        }

        //再载入未选
        for (auto temp : groupList) {
            if (find(conf_groupList.begin(), conf_groupList.end(), temp.group_id) == conf_groupList.end())
                list_groupList.at(0).append({temp.group_name, to_string(temp.group_id)});
        }

        //勾选已选的
        auto size = list_groupList.size_item(0);
        for (int i = 0; i < size; i++) {
            string buf = list_groupList.at(0).at(i).text(1);

            if (find(conf.alone[conf_index].relayGroupList.begin(),
                     conf.alone[conf_index].relayGroupList.end(),
                     atoll(buf.c_str()))
                != conf.alone[conf_index].relayGroupList.end()) {
                list_groupList.at(0).at(i).check(true);
            }
        }

        //转发格式
        text_relayGroupWord.reset(conf.alone[conf_index].relayGroupWord);

        text_relayGroupMsg_trimFront.reset(to_string(conf.alone[conf_index].relayGroupMsg_trimFront));
        text_relayGroupMsg_trimBack.reset(to_string(conf.alone[conf_index].relayGroupMsg_trimBack));
    }

    //显示文本
    void showText(string title, string word) {
        form fm_temp;
        fm_temp.caption(title);

        textbox text{fm_temp};
        text.caption(word);
        text.editable(false);

        fm_temp.div("<text>");
        fm_temp["text"] << text;
        fm_temp.collocate();

        fm_temp.show();
        exec();
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            "<vert"
            "<"
            //群列表
            "<vert <weight=25 lab_groupList> <list_groupList>>"
            "<weight=10>"

            //转发格式
            "<vert <weight=25 <lab_relayGroupWord> <margin = [0,0,7] button_variable>> <text_relayGroupWord>>"
            "<weight=10>"
            ">"

            "<weight=20>"
            "<weight=25%"
            //消息修剪
            "<vert <weight=25 lab_relayGroupMsg_trim> <vert <weight=25 <weight=80 "
            "lab_relayGroupMsg_trimFront><text_relayGroupMsg_trimFront>> <weight=25 <weight=80 "
            "lab_relayGroupMsg_trimBack><text_relayGroupMsg_trimBack>>>>"

            "<weight=10%>"

            //保存
            "<weight=40% button_save>"
            ">"
            ">");

        //群列表
        lab_groupList.create(*this);
        lab_groupList.caption(u8"触发后转发到群:");
        place_.field("lab_groupList") << lab_groupList;

        list_groupList.create(*this);
        list_groupList.checkable(true);
        list_groupList.append_header(u8"群名");
        list_groupList.append_header(u8"群号码");
        place_.field("list_groupList") << list_groupList;

        //消息修剪
        lab_relayGroupMsg_trim.create(*this);
        lab_relayGroupMsg_trim.caption(u8"消息修剪（去广告尾巴专用）");
        place_.field("lab_relayGroupMsg_trim") << lab_relayGroupMsg_trim;

        lab_relayGroupMsg_trimFront.create(*this);
        lab_relayGroupMsg_trimFront.caption(u8"删除前面行数:");
        place_.field("lab_relayGroupMsg_trimFront") << lab_relayGroupMsg_trimFront;

        text_relayGroupMsg_trimFront.create(*this);
        text_relayGroupMsg_trimFront.line_wrapped(false);
        text_relayGroupMsg_trimFront.multi_lines(false);
        place_.field("text_relayGroupMsg_trimFront") << text_relayGroupMsg_trimFront;

        lab_relayGroupMsg_trimBack.create(*this);
        lab_relayGroupMsg_trimBack.caption(u8"删除后面行数:");
        place_.field("lab_relayGroupMsg_trimBack") << lab_relayGroupMsg_trimBack;

        text_relayGroupMsg_trimBack.create(*this);
        text_relayGroupMsg_trimBack.line_wrapped(false);
        text_relayGroupMsg_trimBack.multi_lines(false);
        place_.field("text_relayGroupMsg_trimBack") << text_relayGroupMsg_trimBack;

        //转发消息格式
        lab_relayGroupWord.create(*this);
        lab_relayGroupWord.caption(u8"转发消息格式:");
        place_.field("lab_relayGroupWord") << lab_relayGroupWord;

        text_relayGroupWord.create(*this);
        text_relayGroupWord.line_wrapped(true);
        text_relayGroupWord.tip_string(u8"不填写则转发消息原格式 即 {msg}");
        place_.field("text_relayGroupWord") << text_relayGroupWord;

        //变量按钮
        button_variable.create(*this);
        button_variable.caption(u8"可用变量");
        button_variable.events().click([this] {
            string variable(
                u8"{msg}\t\t触发关键词的消息内容\r\n"
                u8"{日期}\t\t当前日期\r\n"
                u8"{时间}\t\t当前时间\r\n"
                u8"{星期}\t\t当前星期\r\n"
                u8"{关键词}\t\t触发的关键词\r\n"
                u8"{QQ号码}\t触发关键词的QQ号码\r\n"
                u8"{QQ名称}\t触发关键词的QQ名称\r\n"
                u8"{QQ名片}\t触发关键词的QQ名片\r\n"
                u8"{群号码}\t触发关键词的群号码\r\n"
                u8"{群名称}\t触发关键词的群名称\r\n"
                u8"\r\n更多变量欢迎进群补充(群:839067703)");

            showText(u8"转发群消息变量", variable);
        });
        place_.field("button_variable") << button_variable;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] {
            //群名单
            conf.alone[conf_index].relayGroupList.clear();
            for (auto temp : list_groupList.checked()) {
                string buf = list_groupList.at(temp).text(1);

                conf.alone[conf_index].relayGroupList.push_back(atoll(buf.c_str()));
            }

            //转发格式
            conf.alone[conf_index].relayGroupWord = text_relayGroupWord.text();

            conf.alone[conf_index].relayGroupMsg_trimFront = atoi(text_relayGroupMsg_trimFront.text().c_str());
            conf.alone[conf_index].relayGroupMsg_trimBack = atoi(text_relayGroupMsg_trimBack.text().c_str());

            writeConf();

            msgbox m_save{*this, u8"成功"};
            m_save << u8"保存成功";
            m_save.show();

            readConf();
        });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_relayGroupMsg(window wd, int index = 0) : panel<false>(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //群列表
    label lab_groupList;
    listbox list_groupList;

    //消息修建
    label lab_relayGroupMsg_trim;
    label lab_relayGroupMsg_trimFront;
    label lab_relayGroupMsg_trimBack;
    textbox text_relayGroupMsg_trimFront;
    textbox text_relayGroupMsg_trimBack;

    //变量
    label lab_relayGroupWord;
    textbox text_relayGroupWord;

    //变量按钮
    button button_variable;

    //保存
    button btn_save;
};

//单独设置
class tab_page_alone : public panel<false> {
private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    void writeList() {
        auto size = list_aloneList.size_item(0);
        for (int i = 0; i < size; i++) {
            string buf = list_aloneList.at(0).at(i).text(0);
            int id = atoi(buf.c_str());

            conf.alone[id].use = list_aloneList.at(0).at(i).checked();
        }
    }

    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        auto groupList = cq::get_group_list();

        vector<string> dealTypeList{"不作处理", "禁言", "踢出", "踢出并拉黑"};

        list_aloneList.erase();
        for (auto tempAlone : conf.alone) {
            if (tempAlone.first == 0) continue;

            string groupListWord;
            dealTypeList[1] = "禁言" + to_string(tempAlone.second.banTimeLen) + "分钟";

            int i = 0;
            for (auto id : tempAlone.second.groupList) {
                string groupName;
                for (auto& group : groupList) {
                    if (group.group_id == id) {
                        groupName = group.group_name;
                        break;
                    }
                }

                groupListWord += groupName + " ";
                if (i > 5) break;

                i++;
            }

            list_aloneList.at(0).append({to_string(tempAlone.first),
                                         to_string(tempAlone.second.priority),
                                         tempAlone.second.name,
                                         groupListWord,
                                         dealTypeList[tempAlone.second.dealType]});
        }

        //勾选已选的
        auto size = list_aloneList.size_item(0);
        for (int i = 0; i < size; i++) {
            string buf = list_aloneList.at(0).at(i).text(0);
            int id = atoi(buf.c_str());

            if (conf.alone[id].use) {
                list_aloneList.at(0).at(i).check(true);
            }
        }
    }

    void openAlone(int conf_index) {
        form fm(*this);
        fm.caption(u8"单独设置");
        fm.size(nana::size(550, 400));

        place place;
        place.bind(fm);
        place.div("<vert <weight=30 tabbar> <tab_frame>>");

        std::vector<std::shared_ptr<panel<false>>> tabpages;
        tabbar<std::string> tabbar{fm};

        //不知道为啥 在添加前禁用scroll 就可以实现显示最前面的scroll页面
        tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, false);

        tabbar.push_back(u8"主要设置");
        tabpages.push_back(std::make_shared<tab_page_aloneMain>(fm, conf_index));
        tabbar.push_back(u8"全局设置");
        tabpages.push_back(std::make_shared<tab_page_overall>(fm, conf_index));
        tabbar.push_back(u8"关键词");
        tabpages.push_back(std::make_shared<tab_page_keyWord>(fm, conf_index));
        tabbar.push_back(u8"过滤名单");
        tabpages.push_back(std::make_shared<tab_page_list>(fm, conf_index));
        tabbar.push_back(u8"触发回复");
        tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm, conf_index));
        tabbar.push_back(u8"转发到群");
        tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm, conf_index));

        std::size_t index = 0;
        for (auto& i : tabpages) {
            tabbar.attach(index++, *i);
            place.field("tab_frame").fasten(*i);
        }

        tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, true);
        tabbar.toolbox(nana::tabbar<std::string>::kits::list, true);
        tabbar.activated(0);
        place.field("tabbar") << tabbar;
        place.collocate();

        fm.modality();
    }

    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            "margin = [15,15,15,15] "

            "<vert"

            //群列表
            "<list_aloneList>"

            //优先级提示
            "<weight=25 lab_priorityTip>"

            ">");

        //列表
        list_aloneList.create(*this);
        list_aloneList.checkable(true);
        list_aloneList.append_header(u8"id", 50);
        list_aloneList.append_header(u8"优先级", 50);
        list_aloneList.append_header(u8"设置名称");
        list_aloneList.append_header(u8"启用群", 150);
        list_aloneList.append_header(u8"处理方法");

        place_.field("list_aloneList") << list_aloneList;

        list_aloneList.events().mouse_up([this](const arg_mouse& arg) {
            //如果为点击则重新保存复选框
            if (mouse::right_button != arg.button) {
                auto size = list_aloneList.size_item(0);

                writeList();
                writeConf();
                readConf();

                return;
            }

            auto index = list_aloneList.selected();

            //菜单
            menu menu_;

            //点击菜单事件

            //添加
            auto handlerAdd = [=](menu::item_proxy& ip) {
                int add_index = conf.alone.size();

                if (conf.alone.find(add_index) != conf.alone.end()) {
                    msgbox m_error(*this, u8"错误");
                    m_error.icon(msgbox::icon_error);
                    m_error << u8"配置项id冲突，添加失败，请尝试重启酷Q";
                    m_error.show();
                    return;
                }

                openAlone(add_index);
                conf.alone[add_index].use = true;

                writeList();
                writeConf();
                readConf();
            };

            //编辑
            auto handlerEdit = [=](menu::item_proxy& ip) {
                int conf_index = atoi(list_aloneList.at(index.at(0)).text(0).c_str());

                openAlone(conf_index);

                writeList();
                writeConf();
                readConf();
            };

            //删除
            auto handlerDelete = [=](menu::item_proxy& ip) {
                msgbox m{*this, u8"确认", nana::msgbox::yes_no};
                m << u8"是否删除此项";
                auto res = m.show();

                if (res == m.pick_yes) {
                    int conf_index = atoi(list_aloneList.at(index.at(0)).text(0).c_str());
                    conf.alone.erase(conf_index);

                    writeConf();
                    readConf();
                }
            };

            //右击内容
            if (index.size()) {
                menu_.append(u8"添加", handlerAdd);
                menu_.append(u8"编辑", handlerEdit);
                menu_.append(u8"删除", handlerDelete);

                menu_.popup_await(*this, arg.pos.x, arg.pos.y);
            }
            //右击空白
            else {
                menu_.append(u8"添加", handlerAdd);

                menu_.popup_await(*this, arg.pos.x, arg.pos.y);
            }
        });

        //优先级提示
        lab_priorityTip.create(*this);
        lab_priorityTip.caption(u8"提示:   优先级数值小 > 优先级数值大 > 默认的设置");
        place_.field("lab_priorityTip") << lab_priorityTip;

        readConf();
    }

public:
    tab_page_alone(window wd) : panel<false>(wd) {
        init();
    }

private:
    place place_;

    //列表
    listbox list_aloneList;

    //优先级提示
    label lab_priorityTip;
};

Gui::Gui() {
}

//打开主要界面
void Gui::openMain() {
    form fm;
    fm.caption(u8"设置");
    fm.size(nana::size(550, 400));

    place place;
    place.bind(fm);
    place.div("<vert <weight=30 tabbar> <tab_frame>>");

    std::vector<std::shared_ptr<panel<false>>> tabpages;
    tabbar<std::string> tabbar{fm};

    //不知道为啥 在添加前禁用scroll 就可以实现显示最前面的scroll页面
    tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, false);

    tabbar.push_back(u8"主要设置");
    tabpages.push_back(std::make_shared<tab_page_main>(fm));
    tabbar.push_back(u8"全局设置");
    tabpages.push_back(std::make_shared<tab_page_overall>(fm));
    tabbar.push_back(u8"关键词");
    tabpages.push_back(std::make_shared<tab_page_keyWord>(fm));
    tabbar.push_back(u8"过滤名单");
    tabpages.push_back(std::make_shared<tab_page_list>(fm));
    tabbar.push_back(u8"触发回复");
    tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm));
    tabbar.push_back(u8"转发到群");
    tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm));
    tabbar.push_back(u8"单独设置");
    tabpages.push_back(std::make_shared<tab_page_alone>(fm));

    std::size_t index = 0;
    for (auto& i : tabpages) {
        tabbar.attach(index++, *i);
        place.field("tab_frame").fasten(*i);
    }

    tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, true);
    tabbar.toolbox(nana::tabbar<std::string>::kits::list, true);
    tabbar.activated(0);
    place.field("tabbar") << tabbar;
    place.collocate();
    fm.show();
    exec();
}