#include "gui.h"
#include "mycq.hpp"

#include <Windows.h>

#include "myJson.h"
#include "mynetwork.h"

//#include <regex>
#include <boost/regex.hpp>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>

using namespace nana;
using namespace std;

//#define BG_COLOE 0xe6e6e6
#define GROUP_COLOE 0xc9c5be

extern MyJson conf;

void openUrl(std::string url) {
    string temp = "\"" + url;
    temp += "\"";

    ShellExecuteA(NULL, "open", "explorer.exe", temp.c_str(), NULL, SW_SHOW);
}

//窗口父类
class tab_father : public panel<false> {
protected:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    //显示文本
    void showText(string title, string word) {
        form fm_temp;
        fm_temp.caption(title);
        //fm_temp.bgcolor(color_rgb(BG_COLOE));

        textbox text{fm_temp};
        text.caption(word);
        text.editable(false);

        fm_temp.div("<text>");
        fm_temp["text"] << text;
        fm_temp.collocate();

        fm_temp.show();
        exec();
    }

public:
    tab_father(window wd) : panel<false>(wd), isSave(true) {
        color_group=color_rgb(GROUP_COLOE);
        // this->bgcolor(color_rgb(BG_COLOE));
    }

    //获取保存状态
    bool getSave() {
        return isSave;
    }

    //读取配置
    virtual void readConf() {
        msgbox m_error{*this, u8"错误"};
        m_error.icon(msgbox::icon_error);
        m_error << u8"非法访问";
        m_error.show();
    }

    //保存
    virtual bool save() {
        msgbox m_error{*this, u8"错误"};
        m_error.icon(msgbox::icon_error);
        m_error << u8"非法访问";
        m_error.show();
        return false;
    }

protected:
    bool isSave;
    color color_group;
};

//主要设置
class tab_page_main : public tab_father {
protected:
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

        isSave = true;
    }

public:
    bool save() {
        bool ok = boost::regex_match(text_admin.text(), boost::regex("[\\n\\d\\r]*"));
        if (!ok) {
            msgbox m_error{*this, u8"错误"};
            m_error.icon(msgbox::icon_error);
            m_error << u8"主人QQ 格式有误";
            m_error.show();
            return false;
        }

        //主人QQ
        auto line = text_admin.text_line_count();
        conf.admin.clear();
        for (size_t i = 0; i < line; i++) {
            string buf;
            text_admin.getline(i, buf);
            if (!buf.empty()) conf.admin.insert(atoll(buf.c_str()));
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

        isSave = true;
        return true;
    }

private:
    //初始化
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [15,15,15,15] "

            //主人QQ
            "<group_admin>"
            //"<weight=10>"

            //其他
            "<vert"
            "<weight=40% group_other>"

            // //绑定秘钥
            // "<weight=25 lab_usingKey>"
            // "<weight=25 <text_usingKey><weight=60 button_usingKey>>"
            "<>"

            //保存
            "<margin = [15,15,15,15] button_save>"

            ">");

        //主人QQ
        group_admin.create(*this);
        group_admin.bgcolor(color_group);
        group_admin.caption(u8"主人QQ(每行一个):");
        group_admin.div("margin = [5] <text_admin>");
        place_.field("group_admin") << group_admin;

        text_admin.create(group_admin);
        text_admin.events().text_changed([this]() { isSave = false; });
        group_admin["text_admin"] << text_admin;

        //其他
        group_other.create(*this);
        group_other.bgcolor(color_group);
        group_other.caption(u8"其他");
        group_other.div(
            "<vert "

            //指令前缀
            "<weight=25 <weight=25% lab_prefix><text_prefix><weight=20%>>"
            //"<weight=10>"

            //开关
            "<vert <><check><>>"

            //使用教程 //在线更新 //反馈
            "<<button_document> <button_update> <button_feedback>>"
            ">");
        place_.field("group_other") << group_other;

        //指令前缀
        lab_prefix.create(group_other);
        lab_prefix.bgcolor(color_group);
        lab_prefix.caption(u8"指令前缀：");
        group_other["lab_prefix"] << lab_prefix;

        text_prefix.create(group_other);
        text_prefix.line_wrapped(false);
        text_prefix.multi_lines(false);
        text_prefix.events().text_changed([this]() { isSave = false; });
        group_other["text_prefix"] << text_prefix;

        //收到私聊消息转发给主人
        check_relayPrivateMsg.create(group_other);
        check_relayPrivateMsg.bgcolor(color_group);
        check_relayPrivateMsg.caption(u8"收到的私聊消息转发给主人");
        check_relayPrivateMsg.events().click([this]() { isSave = false; });
        group_other["check"] << check_relayPrivateMsg;

        button_document.create(group_other);
        button_document.caption(u8"使用教程 待开发");
        button_document.events().click([this] { openUrl("https://jq.qq.com/?_wv=1027&k=5Q20l6I"); });
        group_other["button_document"] << button_document;

        button_update.create(group_other);
        button_update.caption(u8"检查更新");
        button_update.events().click([this] {
            Update up;
            string inf;
            Update::updateType type = up.check(inf);

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
        group_other["button_update"] << button_update;

        button_feedback.create(group_other);
        button_feedback.caption(u8"问题反馈 待开发");
        button_feedback.events().click([this] { openUrl("https://jq.qq.com/?_wv=1027&k=5Q20l6I"); });
        group_other["button_feedback"] << button_feedback;

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
        btn_save.events().click([this] { save(); });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_main(window wd) : tab_father(wd) {
        init();
    }

private:
    place place_;
    int conf_index;

    //主人QQ
    group group_admin;
    textbox text_admin;

    //其他
    group group_other;

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
class tab_page_aloneMain : public tab_father {
protected:
    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        text_name.reset(conf.alone[conf_index].name);
        text_priority.from(conf.alone[conf_index].priority);

        isSave = true;
    }

public:
    bool save() {
        string priority_temp = text_priority.text();
        bool ok = boost::regex_match(priority_temp, boost::regex("[0-9]+"));
        if (!ok) {
            msgbox m_error{*this, u8"错误"};
            m_error.icon(msgbox::icon_error);
            m_error << u8"优先级 格式有误";
            m_error.show();
            return false;
        }

        conf.alone[conf_index].priority = atoi(priority_temp.c_str());
        conf.alone[conf_index].name = text_name.text();

        writeConf();

        msgbox m_save{*this, u8"成功"};
        m_save << u8"保存成功";
        m_save.show();

        //重新载入配置
        readConf();

        isSave = true;
        return true;
    }

private:
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
        // lab_priority.bgcolor(color_group);
        lab_priority.caption(u8"优先级：");
        place_.field("lab_priority") << lab_priority;

        text_priority.create(*this);
        text_priority.line_wrapped(false);
        text_priority.multi_lines(false);
        text_priority.events().text_changed([this]() { isSave = false; });
        place_.field("text_priority") << text_priority;

        //设置名称
        lab_name.create(*this);
        // lab_name.bgcolor(color_group);
        lab_name.caption(u8"设置名称：");
        place_.field("lab_name") << lab_name;

        text_name.create(*this);
        text_name.line_wrapped(false);
        text_name.multi_lines(false);
        text_name.events().text_changed([this]() { isSave = false; });
        place_.field("text_name") << text_name;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] { save(); });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_aloneMain(window wd, int index = 0) : tab_father(wd), conf_index(index) {
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
class tab_page_overall : public tab_father {
protected:
    void readConf() {
        conf.file2json();
        conf.json2all();

        //载入群名单
        auto groupList = mycq::get_group_list_map();

        //清空原有
        list_groupList.erase();

        auto& conf_groupList = conf.alone[conf_index].groupList;
        //先载入已选
        for (auto id : conf_groupList) {
            list_groupList.at(0).append({groupList[id].group_name, to_string(id)});
        }

        //再载入未选
        for (auto temp : groupList) {
            if (find(conf_groupList.begin(), conf_groupList.end(), temp.first) == conf_groupList.end())
                list_groupList.at(0).append({temp.second.group_name, to_string(temp.first)});
        }

        //勾选已选的
        auto size = list_groupList.size_item(0);
        for (size_t i = 0; i < size; i++) {
            string buf = list_groupList.at(0).at(i).text(1);

            if (find(conf.alone[conf_index].groupList.begin(),
                     conf.alone[conf_index].groupList.end(),
                     atoll(buf.c_str()))
                != conf.alone[conf_index].groupList.end()) {
                list_groupList.at(0).at(i).check(true);
            }
        }

        //处理方式
        check_deal.at(conf.alone[conf_index].dealType)->check(true);

        //禁言时长
        text_banTimeLen.reset(to_string(conf.alone[conf_index].banTimeLen));

        //一些开关
        check_deleteMsg.check(conf.alone[conf_index].deleteMsg);
        check_streng.check(conf.alone[conf_index].streng);
        check_deleteCQCode.check(conf.alone[conf_index].deleteCQCode);
        check_keyWordSendAdmin.check(conf.alone[conf_index].keyWordSendAdmin);

        isSave = true;
    }

public:
    bool save() {
        bool ok = boost::regex_match(text_banTimeLen.text(), boost::regex("[0-9]*"));
        if (!ok) {
            msgbox m_error{*this, u8"错误"};
            m_error.icon(msgbox::icon_error);
            m_error << u8"禁言时长 格式有误";
            m_error.show();
            return false;
        }

        //群名单
        conf.alone[conf_index].groupList.clear();
        for (auto temp : list_groupList.checked()) {
            string buf = list_groupList.at(temp).text(1);

            conf.alone[conf_index].groupList.insert(atoll(buf.c_str()));
        }

        //处理方式
        int dealType = 0;
        for (size_t i = 0; i < check_deal.size(); i++) {
            if (check_deal.at(i)->checked()) {
                dealType = i;
                break;
            }
        }
        conf.alone[conf_index].dealType = dealType;

        //禁言时长
        conf.alone[conf_index].banTimeLen = atoi(text_banTimeLen.text().c_str());

        //一些开关
        conf.alone[conf_index].deleteMsg = check_deleteMsg.checked();
        conf.alone[conf_index].streng = check_streng.checked();
        conf.alone[conf_index].deleteCQCode = check_deleteCQCode.checked();
        conf.alone[conf_index].keyWordSendAdmin = check_keyWordSendAdmin.checked();

        //写入配置
        writeConf();

        msgbox m_save{*this, u8"成功"};
        m_save << u8"保存成功";
        m_save.show();

        //重新载入配置
        readConf();

        isSave = true;
        return true;
    }

private:
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [5] "

            //监控群
            "<group_list>"
            //"<weight=20>"

            "<vert"
            //处理方式
            "<weight=40% group_deal>"
            //"<weight=10>"

            //一些开关
            "<weight=40% group_switch>"
            //"<weight=10>"

            //保存按钮
            "<weight=10% button_save>"

            ">");

        //监控群
        group_list.create(*this);
        group_list.bgcolor(color_group);
        group_list.caption(u8"监控群:");
        group_list.div(
            "margin = [5] <vert <list_groupList>"
            "<weight=25 <text_groupAdd><weight=40% button_groupAdd>><weight=5>"
            "<weight=30 gap=[10,10] arrange=[40,40] button_list> >");
        place_.field("group_list") << group_list;

        list_groupList.create(group_list);
        list_groupList.checkable(true);
        list_groupList.append_header(u8"群名");
        list_groupList.append_header(u8"群号码");
        list_groupList.events().checked([this]() { isSave = false; });
        group_list["list_groupList"] << list_groupList;

        //手动添加群
        text_groupAdd.create(group_list);
        text_groupAdd.line_wrapped(false);
        text_groupAdd.multi_lines(false);
        group_list["text_groupAdd"] << text_groupAdd;

        button_groupAdd.create(group_list);
        button_groupAdd.caption(u8"手动添加");
        button_groupAdd.events().click([this] {
            isSave = false;

            bool ok = boost::regex_match(text_groupAdd.text(), boost::regex("[1-9][0-9]*"));
            if (!ok) {
                msgbox m_error{*this, u8"错误"};
                m_error.icon(msgbox::icon_error);
                m_error << u8"格式有误";
                m_error.show();
                return;
            }

            list_groupList.at(0).append({"", text_groupAdd.text()});

            //查找添加项并勾选
            auto size = list_groupList.size_item(0);
            for (size_t i = 0; i < size; i++) {
                string buf = list_groupList.at(0).at(i).text(1);

                if (buf == text_groupAdd.text()) {
                    list_groupList.at(0).at(i).check(true);
                    break;
                }
            }
        });
        group_list["button_groupAdd"] << button_groupAdd;

        //列表按钮
        //全选
        button_listAll.create(group_list);
        button_listAll.caption(u8"全选");
        button_listAll.events().click([this] {
            isSave = false;

            auto size = list_groupList.size_item(0);
            for (size_t i = 0; i < size; i++) {
                list_groupList.at(listbox::index_pair(0, i)).check(true);
            }
        });
        //反选
        button_listReverse.create(group_list);
        button_listReverse.caption(u8"反选");
        button_listReverse.events().click([this] {
            isSave = false;

            auto size = list_groupList.size_item(0);
            for (size_t i = 0; i < size; i++) {
                auto p = list_groupList.at(listbox::index_pair(0, i));

                p.check(!p.checked());
            }
        });
        group_list["button_list"] << button_listAll << button_listReverse;

        //处理方式
        group_deal.create(*this);
        group_deal.bgcolor(color_group);
        group_deal.caption(u8"处理方式:");
        group_deal.div(
            "margin = [5] <vert"
            "<vert check_deal>"
            "<weight=25 <weight=45% label_banTimeLen> <weight=5% text_banTimeLen> <label_banTimeLenTip>> >");
        place_["group_deal"] << group_deal;

        vector<string> groupStr;
        groupStr.push_back(u8"不做处理");
        groupStr.push_back(u8"禁言");
        groupStr.push_back(u8"踢出");
        groupStr.push_back(u8"拉黑并踢出");
        for (size_t i = 0; i < groupStr.size(); i++) {
            auto p = std::make_shared<checkbox>(group_deal);
            p->bgcolor(color_group);
            p->events().checked([this]() { isSave = false; });
            check_deal.push_back(p);

            // Add the checkbox to the radio group. The radio group does not
            // manage the life of checkboxs.
            radio_group_deal.add(*p);
            group_deal["check_deal"] << *p;

            p->caption(groupStr[i]);
            p->events().click([this]() {
                std::size_t index = this->radio_group_deal.checked();
                std::string str = this->check_deal[index]->caption();

                if (str == u8"禁言") {
                    label_banTimeLenTip.caption(u8"分钟");
                } else {
                    label_banTimeLenTip.caption(u8"分钟(禁言方式下生效)");
                }
            });
        }

        label_banTimeLen.create(group_deal);
        label_banTimeLen.bgcolor(color_group);
        label_banTimeLen.caption(u8"禁言时长:");
        group_deal["label_banTimeLen"] << label_banTimeLen;

        text_banTimeLen.create(group_deal);
        text_banTimeLen.line_wrapped(false);
        text_banTimeLen.multi_lines(false);
        text_banTimeLen.events().text_changed([this]() { isSave = false; });
        group_deal["label_banTimeLen"] << text_banTimeLen;

        label_banTimeLenTip.create(group_deal);
        label_banTimeLenTip.bgcolor(color_group);
        label_banTimeLenTip.caption(u8"分钟");
        group_deal["label_banTimeLenTip"] << label_banTimeLenTip;

        //一些功能开关
        group_switch.create(*this);
        group_switch.bgcolor(color_group);
        group_switch.caption(u8"一些功能开关:");
        group_switch.div("<vert margin = [5] check_switch>");
        place_.field("group_switch") << group_switch;

        check_streng.create(group_switch);
        check_streng.bgcolor(color_group);
        check_streng.caption(u8"强力检测关键词");
        check_streng.events().checked([this]() { isSave = false; });
        group_switch["check_switch"] << check_streng;

        check_deleteMsg.create(group_switch);
        check_deleteMsg.bgcolor(color_group);
        check_deleteMsg.caption(u8"撤回触发关键词消息（需Pro）");
        check_deleteMsg.events().checked([this]() { isSave = false; });
        group_switch["check_switch"] << check_deleteMsg;

        check_deleteCQCode.create(group_switch);
        check_deleteCQCode.bgcolor(color_group);
        check_deleteCQCode.caption(u8"过滤CQ码（如图片消息，签到消息等）");
        check_deleteCQCode.events().checked([this]() { isSave = false; });
        group_switch["check_switch"] << check_deleteCQCode;

        check_keyWordSendAdmin.create(group_switch);
        check_keyWordSendAdmin.bgcolor(color_group);
        check_keyWordSendAdmin.caption(u8"触发关键词提醒主人");
        check_keyWordSendAdmin.events().checked([this]() { isSave = false; });
        group_switch["check_switch"] << check_keyWordSendAdmin;

        button_save.create(*this);
        button_save.caption(u8"保存");
        button_save.events().click([this] { save(); });
        place_.field("button_save") << button_save;
        group_list.collocate();

        //读取配置
        readConf();
    }

public:
    tab_page_overall(window wd, int index = 0) : tab_father(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //监控群
    group group_list;
    listbox list_groupList; //群列表

    textbox text_groupAdd; //手动添加群	文本框
    button button_groupAdd; //手动添加群	按钮

    button button_listReverse; //反选
    button button_listAll; //全选

    //处理方式
    group group_deal;
    std::vector<std::shared_ptr<checkbox>> check_deal;
    radio_group radio_group_deal;
    label label_banTimeLen; //禁言时间 标签
    label label_banTimeLenTip; //禁言时间 提示标签
    textbox text_banTimeLen; //禁言时间 文本框

    //一些功能开关
    group group_switch;
    // vector<std::shared_ptr<checkbox>> check_switchs;
    checkbox check_streng; //强力检测
    checkbox check_deleteMsg; //撤回消息
    checkbox check_deleteCQCode; //过滤CQ码
    checkbox check_keyWordSendAdmin; //触发关键词提醒主人

    //保存
    button button_save;
};

//列表
class tab_page_list : public tab_father {
protected:
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
        if (conf.alone[conf_index].QQListType == 0) {
            lab_QQListTypeTip.caption(u8"当前为白名单模式，不会监控名单中的QQ");
            group_QQList.caption(u8"QQ白名单(每行一个):");
        } else if (conf.alone[conf_index].QQListType == 1) {
            lab_QQListTypeTip.caption(u8"当前为监控名单模式，只监控名单中的QQ");
            group_QQList.caption(u8"QQ监控名单(每行一个):");
        }

        isSave = true;
    }

public:
    bool save() {
        bool ok = boost::regex_match(text_QQList.text(), boost::regex("[\\n\\d\\r]*"));
        if (!ok) {
            msgbox m_error{*this, u8"错误"};
            m_error.icon(msgbox::icon_error);
            m_error << u8"QQ白名单/监控名单 格式有误";
            m_error.show();
            return false;
        }

        // QQ特殊名单
        auto QQList_line = text_QQList.text_line_count();
        conf.alone[conf_index].QQList.clear();
        for (size_t i = 0; i < QQList_line; i++) {
            string buf;
            text_QQList.getline(i, buf);
            if (!buf.empty()) conf.alone[conf_index].QQList.insert(atoll(buf.c_str()));
        }

        //白名单关键词
        auto keyWrodWhite_line = text_keyWordWhite.text_line_count();
        conf.alone[conf_index].keyWordWhite.clear();
        for (size_t i = 0; i < keyWrodWhite_line; i++) {
            string buf;
            text_keyWordWhite.getline(i, buf);
            if (!buf.empty()) conf.alone[conf_index].keyWordWhite.insert(buf.c_str());
        }

        //特殊名单类型  白名单||监控名单
        int listType = 0;
        for (size_t i = 0; i < check_QQListType.size(); i++) {
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

        isSave = true;
        return true;
    }

private:
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [15,15,15,15]"

            //左边
            "<vert"

            //特殊QQ名单
            "<group_QQList>"
            //"<weight=10>"
            ">"

            //右边
            "<vert"

            //白名单关键词
            "<group_keyWordWhite>"
            //"<weight=10>"

            //保存
            "<weight=20% button_save>"

            ">");

        //白名单关键词
        group_keyWordWhite.create(*this);
        group_keyWordWhite.bgcolor(color_group);
        group_keyWordWhite.caption(u8"白名单关键词(每行一个):");
        group_keyWordWhite.div("margin = [5] <text_keyWordWhite>");
        place_.field("group_keyWordWhite") << group_keyWordWhite;

        text_keyWordWhite.create(group_keyWordWhite);
        text_keyWordWhite.line_wrapped(true);
        text_keyWordWhite.tip_string(u8"包含这些关键词的消息将不会检测");
        text_keyWordWhite.events().text_changed([this]() { isSave = false; });
        group_keyWordWhite["text_keyWordWhite"] << text_keyWordWhite;

        //特殊QQ名单
        group_QQList.create(*this);
        group_QQList.bgcolor(color_group);
        group_QQList.caption(u8"QQ 白名单/监控名单(每行一个):");
        group_QQList.div("vert margin = [5] <text_QQList> <weight=25 check_QQListType> <weight=25 lab_QQListTypeTip>");
        place_.field("group_QQList") << group_QQList;

        text_QQList.create(group_QQList);
        text_QQList.line_wrapped(true);
        text_QQList.events().text_changed([this]() { isSave = false; });
        group_QQList["text_QQList"] << text_QQList;

        //设置特殊名单为  白名单 || 监控名单
        lab_QQListTypeTip.create(group_QQList);
        lab_QQListTypeTip.bgcolor(color_group);
        group_QQList["lab_QQListTypeTip"] << lab_QQListTypeTip;

        vector<string> groupStr;
        groupStr.push_back(u8"白名单");
        groupStr.push_back(u8"监控名单");

        for (size_t i = 0; i < groupStr.size(); i++) {
            auto p = std::make_shared<checkbox>(group_QQList);
            p->bgcolor(color_group);
            p->events().checked([this]() { isSave = false; });
            check_QQListType.push_back(p);

            group_QQListType.add(*p);
            group_QQList["check_QQListType"] << *p;

            p->caption(groupStr[i]);
            p->events().click([this]() {
                std::size_t index = this->group_QQListType.checked();
                std::string str = this->check_QQListType[index]->caption();

                if (str == u8"白名单") {
                    lab_QQListTypeTip.caption(u8"当前为白名单模式，不会监控名单中的QQ");
                    group_QQList.caption(u8"QQ白名单(每行一个):");
                } else if (str == u8"监控名单") {
                    lab_QQListTypeTip.caption(u8"当前为监控名单模式，只监控名单中的QQ");
                    group_QQList.caption(u8"QQ监控名单(每行一个):");
                } else {
                    lab_QQListTypeTip.caption("");
                }
            });
        }

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] { save(); });

        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_list(window wd, int index = 0) : tab_father(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //白名单关键词
    group group_keyWordWhite;
    textbox text_keyWordWhite;

    // QQ列表
    group group_QQList;
    textbox text_QQList;

    //特殊QQ名单类型
    std::vector<std::shared_ptr<checkbox>> check_QQListType;
    radio_group group_QQListType;
    label lab_QQListTypeTip;

    //保存
    button btn_save;
};

//关键词
class tab_page_keyWord : public tab_father {
protected:
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

        isSave = true;
    }

public:
    bool save() {
        //关键词
        auto keyWord_line = text_keyWord.text_line_count();
        conf.alone[conf_index].keyWord.clear();
        for (size_t i = 0; i < keyWord_line; i++) {
            string buf;
            text_keyWord.getline(i, buf);
            if (!buf.empty()) conf.alone[conf_index].keyWord.insert(buf.c_str());
        }

        //正则表达式关键词
        auto keyWordRegex_line = text_keyWordRegex.text_line_count();
        conf.alone[conf_index].keyWordRegex.clear();
        for (size_t i = 0; i < keyWordRegex_line; i++) {
            string buf;
            text_keyWordRegex.getline(i, buf);
            if (!buf.empty()) conf.alone[conf_index].keyWordRegex.insert(buf.c_str());
        }

        //写入配置
        writeConf();

        msgbox m_save{*this, u8"成功"};
        m_save << u8"保存成功";
        m_save.show();

        //重新读取配置
        readConf();

        isSave = true;
        return true;
    }

private:
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [15,15,15,15] "

            //普通关键词
            "<group_keyWord>"
            //"<weight=10>"

            //右边
            "<vert"

            //正则表达式关键词
            "<group_keyWordRegex>"
            //"<weight=10>"

            //保存
            "<weight=25% button_save>"

            ">");

        //普通关键词
        group_keyWord.create(*this);
        group_keyWord.bgcolor(color_group);
        group_keyWord.caption(u8"普通关键词(每行一个):");
        group_keyWord.div("margin = [5] <text_keyWord>");
        place_.field("group_keyWord") << group_keyWord;

        text_keyWord.create(group_keyWord);
        text_keyWord.line_wrapped(true);
        text_keyWord.events().text_changed([this]() { isSave = false; });
        group_keyWord["text_keyWord"] << text_keyWord;

        //正则表达式关键词
        group_keyWordRegex.create(*this);
        group_keyWordRegex.bgcolor(color_group);
        group_keyWordRegex.caption(u8"正则表达式关键词(每行一个):");
        group_keyWordRegex.div("margin = [5] <vert <text_keyWordRegex> <weight=20 <><weight=30% btn_regexTest>> >");
        place_.field("group_keyWordRegex") << group_keyWordRegex;

        text_keyWordRegex.create(group_keyWordRegex);
        text_keyWordRegex.line_wrapped(true);
        text_keyWordRegex.events().text_changed([this]() { isSave = false; });
        group_keyWordRegex["text_keyWordRegex"] << text_keyWordRegex;

        //正则表达式测试
        btn_regexTest.create(group_keyWordRegex);
        btn_regexTest.caption(u8"测试正则");
        btn_regexTest.events().click([this] { openUrl("https://c.runoob.com/front-end/854"); });
        group_keyWordRegex["btn_regexTest"] << btn_regexTest;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] { save(); });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_keyWord(window wd, int index = 0) : tab_father(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //主人QQ
    group group_keyWord;
    textbox text_keyWord;

    //正则表达式关键词
    group group_keyWordRegex;
    textbox text_keyWordRegex;

    //正则表达式测试
    button btn_regexTest;

    //保存
    button btn_save;
};

//自定义触发回复
class tab_page_groupWarnWord : public tab_father {
protected:
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

        isSave = true;
    }

public:
    bool save() {
        conf.alone[conf_index].keyWordGroupWarnWord = text_groupWarn.text();
        conf.alone[conf_index].keyWordGroupWarn = check_groupWarn.checked();

        conf.alone[conf_index].keyWordPrivateWarnWord = text_privateWarn.text();
        conf.alone[conf_index].keyWordPrivateWarn = check_privateWarn.checked();

        writeConf();

        msgbox m_save{*this, u8"成功"};
        m_save << u8"保存成功";
        m_save.show();

        readConf();

        isSave = true;
        return true;
    }

private:
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [15,15,15,15] "

            //触发后回复群消息
            "<group_groupWarn>"
            //"<weight=10>"

            //右边
            "<vert"

            //触发后回复私聊消息
            "<group_privateWarn>"
            //"<weight=10>"

            //保存
            "<weight=25% button_save>"

            ">");

        //触发后回复群消息
        group_groupWarn.create(*this);
        group_groupWarn.bgcolor(color_group);
        group_groupWarn.caption(u8"触发后回复群消息内容:");
        group_groupWarn.div(
            "<vert margin = [5] <weight=25 <weight=70% check_groupWarn> <margin = [0,0,7] button_groupVariable>> "
            "<text_groupWarn>>");
        place_.field("group_groupWarn") << group_groupWarn;

        //触发回复群消息开关
        check_groupWarn.create(group_groupWarn);
        check_groupWarn.bgcolor(color_group);
        check_groupWarn.caption(u8"触发关键词发送群消息提醒");
        check_groupWarn.events().checked([this]() { isSave = false; });
        group_groupWarn["check_groupWarn"] << check_groupWarn;

        //内容
        text_groupWarn.create(group_groupWarn);
        text_groupWarn.line_wrapped(true);
        text_groupWarn.tip_string("默认值：\r\n{at} 触发关键词，处理方式:{处理方式}");
        text_groupWarn.events().text_changed([this]() { isSave = false; });
        group_groupWarn["text_groupWarn"] << text_groupWarn;

        //变量按钮
        button_groupVariable.create(group_groupWarn);
        button_groupVariable.caption(u8"可用变量");
        button_groupVariable.events().click([this] {
            string variable(
                u8"{at}\t\t艾特\r\n"
                u8"{msg}\t\t消息内容\r\n"
                u8"{日期}\t\t当前日期\r\n"
                u8"{时间}\t\t当前时间\r\n"
                u8"{星期}\t\t当前星期\r\n"
                u8"{处理方式}\t触发后处理方式\r\n"
                u8"{关键词}\t\t触发的关键词\r\n"
                u8"{QQ号码}\t触发关键词的QQ号码\r\n"
                u8"{QQ名称}\t触发关键词的QQ名称\r\n"
                u8"{QQ名片}\t触发关键词的QQ名片\r\n"
                u8"{群号码}\t触发关键词的群号码\r\n"
                u8"{群名称}\t触发关键词的群名称\r\n"
                u8"\r\n更多变量欢迎进群补充(群:839067703)");
            showText("回复内容变量", variable);
        });
        group_groupWarn["button_groupVariable"] << button_groupVariable;

        //触发后发送私聊消息
        group_privateWarn.create(*this);
        group_privateWarn.bgcolor(color_group);
        group_privateWarn.caption(u8"触发后回复私聊消息内容:");
        group_privateWarn.div(
            "<vert <weight=25 <weight=70% check_privateWarn> <margin = [0,0,7] button_privateVariable>> "
            "<text_privateWarn>>");
        place_.field("group_privateWarn") << group_privateWarn;

        //触发后回复私聊消息开关
        check_privateWarn.create(group_privateWarn);
        check_privateWarn.bgcolor(color_group);
        check_privateWarn.caption(u8"触发关键词发送私聊消息提醒");
        check_privateWarn.events().checked([this]() { isSave = false; });
        group_privateWarn["check_privateWarn"] << check_privateWarn;

        //内容
        text_privateWarn.create(group_privateWarn);
        text_privateWarn.line_wrapped(true);
        text_privateWarn.events().text_changed([this]() { isSave = false; });
        group_privateWarn["text_privateWarn"] << text_privateWarn;

        //变量按钮
        button_privateVariable.create(group_privateWarn);
        button_privateVariable.caption(u8"可用变量");
        button_privateVariable.events().click([this] {
            string variable(
                u8"{msg}\t\t消息内容\r\n"
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
        group_privateWarn["button_privateVariable"] << button_privateVariable;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] { save(); });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_groupWarnWord(window wd, int index = 0) : tab_father(wd), conf_index(index) {
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
    group group_groupWarn;
    textbox text_groupWarn;

    //触发后发送私聊消息
    group group_privateWarn;
    textbox text_privateWarn;

    //变量
    button button_groupVariable;
    button button_privateVariable;

    //保存
    button btn_save;
};

//转发到群
class tab_page_relayGroupMsg : public tab_father {
protected:
    void readConf() {
        conf.file2json();
        conf.json2all();

        //载入群名单
        auto groupList = mycq::get_group_list_map();

        //清空原有
        list_groupList.erase();

        auto& conf_groupList = conf.alone[conf_index].relayGroupList;
        //先载入已选
        for (auto id : conf_groupList) {
            list_groupList.at(0).append({groupList[id].group_name, to_string(id)});
        }

        //再载入未选
        for (auto temp : groupList) {
            if (find(conf_groupList.begin(), conf_groupList.end(), temp.first) == conf_groupList.end())
                list_groupList.at(0).append({temp.second.group_name, to_string(temp.first)});
        }

        //勾选已选的
        auto size = list_groupList.size_item(0);
        for (size_t i = 0; i < size; i++) {
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

        isSave = true;
    }

public:
    bool save() {
        bool ok_text_relayGroupMsg_trimFront =
            boost::regex_match(text_relayGroupMsg_trimFront.text(), boost::regex("[0-9]*"));
        bool ok_text_relayGroupMsg_trimBack =
            boost::regex_match(text_relayGroupMsg_trimBack.text(), boost::regex("[0-9]*"));

        if (!ok_text_relayGroupMsg_trimFront || !ok_text_relayGroupMsg_trimBack) {
            msgbox m_error{*this, u8"错误"};
            m_error.icon(msgbox::icon_error);
            m_error << u8"消息修剪 格式有误";
            m_error.show();
            return false;
        }

        //群名单
        conf.alone[conf_index].relayGroupList.clear();
        for (auto temp : list_groupList.checked()) {
            string buf = list_groupList.at(temp).text(1);

            conf.alone[conf_index].relayGroupList.insert(atoll(buf.c_str()));
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

        isSave = true;
        return true;
    }

private:
    void init() {
        place_.bind(*this);
        place_.div(

            //整体边距
            //"margin = [15,15,15,15] "

            "<vert"
            "<"
            //群列表
            "<group_groupList>"
            //"<weight=10>"

            //转发格式
            "<group_relayGroupWord>"
            ">"

            //"<weight=20>"
            "<weight=25%"
            //消息修剪
            "<group_relayGroupMsg_trim>"

            //"<weight=10%>"

            //保存
            "<weight=40% button_save>"
            ">"
            ">");

        //群列表
        group_groupList.create(*this);
        group_groupList.bgcolor(color_group);
        group_groupList.caption(u8"触发后转发到群:");
        group_groupList.div("vert <list_groupList>");
        place_.field("group_groupList") << group_groupList;

        list_groupList.create(group_groupList);
        list_groupList.checkable(true);
        list_groupList.append_header(u8"群名");
        list_groupList.append_header(u8"群号码");
        list_groupList.events().checked([this]() { isSave = false; });
        group_groupList["list_groupList"] << list_groupList;

        //转发消息格式
        group_relayGroupWord.create(*this);
        group_relayGroupWord.bgcolor(color_group);
        group_relayGroupWord.caption(u8"转发消息格式:");
        group_relayGroupWord.div("vert margin = [5] <text_relayGroupWord> <weight=20 <> <weight=40% button_variable>>");
        place_.field("group_relayGroupWord") << group_relayGroupWord;

        text_relayGroupWord.create(group_relayGroupWord);
        text_relayGroupWord.line_wrapped(true);
        text_relayGroupWord.tip_string(u8"不填写则转发消息原格式 即 {msg}");
        text_relayGroupWord.events().text_changed([this]() { isSave = false; });
        group_relayGroupWord["text_relayGroupWord"] << text_relayGroupWord;

        //变量按钮
        button_variable.create(group_relayGroupWord);
        button_variable.caption(u8"可用变量");
        button_variable.events().click([this] {
            string variable(
                u8"{msg}\t\t消息内容\r\n"
                u8"{日期}\t\t当前日期\r\n"
                u8"{时间}\t\t当前时间\r\n"
                u8"{星期}\t\t当前星期\r\n"
                u8"{关键词}\t\t触发的关键词\r\n"
                u8"{处理方式}\t触发后处理方式\r\n"
                u8"{QQ号码}\t触发关键词的QQ号码\r\n"
                u8"{QQ名称}\t触发关键词的QQ名称\r\n"
                u8"{QQ名片}\t触发关键词的QQ名片\r\n"
                u8"{群号码}\t触发关键词的群号码\r\n"
                u8"{群名称}\t触发关键词的群名称\r\n"
                u8"\r\n更多变量欢迎进群补充(群:839067703)");

            showText(u8"转发群消息变量", variable);
        });
        group_relayGroupWord["button_variable"] << button_variable;

        //消息修剪
        group_relayGroupMsg_trim.create(*this);
        group_relayGroupMsg_trim.bgcolor(color_group);
        group_relayGroupMsg_trim.caption(u8"消息修剪（去广告尾巴专用）");
        group_relayGroupMsg_trim.div(
            "vert <weight=25 <weight=30% lab_relayGroupMsg_trimFront><text_relayGroupMsg_trimFront><weight=15%>> "
            "<weight=25 <weight=30% lab_relayGroupMsg_trimBack><text_relayGroupMsg_trimBack><weight=15%>>");
        place_.field("group_relayGroupMsg_trim") << group_relayGroupMsg_trim;

        lab_relayGroupMsg_trimFront.create(group_relayGroupMsg_trim);
        lab_relayGroupMsg_trimFront.bgcolor(color_group);
        lab_relayGroupMsg_trimFront.caption(u8"删除前面行数:");
        group_relayGroupMsg_trim["lab_relayGroupMsg_trimFront"] << lab_relayGroupMsg_trimFront;

        text_relayGroupMsg_trimFront.create(group_relayGroupMsg_trim);
        text_relayGroupMsg_trimFront.line_wrapped(false);
        text_relayGroupMsg_trimFront.multi_lines(false);
        text_relayGroupMsg_trimFront.events().text_changed([this]() { isSave = false; });
        group_relayGroupMsg_trim["text_relayGroupMsg_trimFront"] << text_relayGroupMsg_trimFront;

        lab_relayGroupMsg_trimBack.create(group_relayGroupMsg_trim);
        lab_relayGroupMsg_trimBack.bgcolor(color_group);
        lab_relayGroupMsg_trimBack.caption(u8"删除后面行数:");
        group_relayGroupMsg_trim["lab_relayGroupMsg_trimBack"] << lab_relayGroupMsg_trimBack;

        text_relayGroupMsg_trimBack.create(group_relayGroupMsg_trim);
        text_relayGroupMsg_trimBack.line_wrapped(false);
        text_relayGroupMsg_trimBack.multi_lines(false);
        text_relayGroupMsg_trimBack.events().text_changed([this]() { isSave = false; });
        group_relayGroupMsg_trim["text_relayGroupMsg_trimBack"] << text_relayGroupMsg_trimBack;

        //保存按钮
        btn_save.create(*this);
        btn_save.caption(u8"保存");
        btn_save.events().click([this] { save(); });
        place_.field("button_save") << btn_save;

        readConf();
    }

public:
    tab_page_relayGroupMsg(window wd, int index = 0) : tab_father(wd), conf_index(index) {
        init();
    }

private:
    place place_;
    int conf_index;

    //群列表
    group group_groupList;
    listbox list_groupList;

    //消息修建
    group group_relayGroupMsg_trim;
    label lab_relayGroupMsg_trimFront;
    label lab_relayGroupMsg_trimBack;
    textbox text_relayGroupMsg_trimFront;
    textbox text_relayGroupMsg_trimBack;

    //转发格式
    group group_relayGroupWord;
    textbox text_relayGroupWord;

    //变量按钮
    button button_variable;

    //保存
    button btn_save;
};

//单独设置
class tab_page_alone : public tab_father {
protected:
    //读取配置
    void readConf() {
        conf.file2json();
        conf.json2all();

        auto groupList = mycq::get_group_list_map();

        vector<string> dealTypeList{"不作处理", "禁言", "踢出", "踢出并拉黑"};

        list_aloneList.erase();
        for (auto tempAlone : conf.alone) {
            if (tempAlone.first == 0) continue;

            string groupListWord;
            dealTypeList[1] = "禁言" + to_string(tempAlone.second.banTimeLen) + "分钟";

            int i = 0;
            for (auto id : tempAlone.second.groupList) {
                groupListWord += groupList[id].group_name + " ";
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
        for (size_t i = 0; i < size; i++) {
            string buf = list_aloneList.at(0).at(i).text(0);
            int id = atoi(buf.c_str());

            if (conf.alone[id].use) {
                list_aloneList.at(0).at(i).check(true);
            }
        }

        isSave = true;
    }

public:
    bool save() {
        isSave = true;
        return true;
    }

private:
    //写入配置
    void writeConf() {
        conf.all2json();
        conf.json2file();
    }

    void writeList() {
        auto size = list_aloneList.size_item(0);
        for (size_t i = 0; i < size; i++) {
            string buf = list_aloneList.at(0).at(i).text(0);
            int id = atoi(buf.c_str());

            conf.alone[id].use = list_aloneList.at(0).at(i).checked();
        }
    }

    void openAlone(int conf_index) {
        form fm(*this);
        fm.caption(u8"单独设置");
        fm.size(nana::size(550, 400));
        // fm.bgcolor(color_rgb(BG_COLOE));

        place place;
        place.bind(fm);
        place.div("<vert <weight=30 tabbar> <tab_frame>>");

        std::vector<std::shared_ptr<tab_father>> tabpages;
        tabbar<std::string> tabbar_{fm};

        //不知道为啥 在添加前禁用scroll 就可以实现显示最前面的scroll页面
        tabbar_.toolbox(nana::tabbar<std::string>::kits::scroll, false);

        tabbar_.push_back(u8"主要设置");
        tabpages.push_back(std::make_shared<tab_page_aloneMain>(fm, conf_index));
        tabbar_.push_back(u8"全局设置");
        tabpages.push_back(std::make_shared<tab_page_overall>(fm, conf_index));
        tabbar_.push_back(u8"关键词");
        tabpages.push_back(std::make_shared<tab_page_keyWord>(fm, conf_index));
        tabbar_.push_back(u8"过滤名单");
        tabpages.push_back(std::make_shared<tab_page_list>(fm, conf_index));
        tabbar_.push_back(u8"触发回复");
        tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm, conf_index));
        tabbar_.push_back(u8"转发到群");
        tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm, conf_index));

        std::size_t index = 0;
        for (auto& i : tabpages) {
            tabbar_.attach(index++, *i);
            place.field("tab_frame").fasten(*i);
        }

        tabbar_.toolbox(nana::tabbar<std::string>::kits::scroll, true);
        tabbar_.toolbox(nana::tabbar<std::string>::kits::list, true);

        tabbar_.events().click([&fm, &tabpages, &tabbar_, this]() {
            // msgbox m{fm, u8"Test"};
            // m << tabpages.at(_tab.item_pos)->getSave();
            // m.show();
            static int lastItem = 0;
            int tempItem = tabbar_.activated();

            //点击列表标签时
            if (!tabpages.at(lastItem)->getSave()) {
                tabbar_.activated(lastItem);

                msgbox inf{fm, u8"未保存", nana::msgbox::yes_no_cancel};
                inf << u8"是否保存已修改的内容";
                auto res = inf.show();

                if (res == nana::msgbox::pick_yes) {
                    if (!tabpages.at(lastItem)->save()) return; //保存失败时
                } else if (res == nana::msgbox::pick_no) {
                    tabpages.at(lastItem)->readConf();
                } else if (res == nana::msgbox::pick_cancel) {
                    return;
                }

                tabbar_.activated(tempItem);
            }

            tabpages.at(tempItem)->readConf();
            lastItem = tempItem;
        });

        //窗口关闭时
        fm.events().unload([&fm, &tabpages, &tabbar_, this](const arg_unload& un) {
            int item = tabbar_.activated();

            if (!tabpages.at(item)->getSave()) {
                msgbox inf{fm, u8"未保存", nana::msgbox::yes_no_cancel};
                inf << u8"是否保存已修改的内容";
                auto res = inf.show();

                if (res == nana::msgbox::pick_yes) {
                    if (!tabpages.at(item)->save()) un.cancel = true; //当保存失败时
                } else if (res == nana::msgbox::pick_no) {
                    tabpages.at(item)->readConf();
                } else if (res == nana::msgbox::pick_cancel) {
                    un.cancel = true;
                }
            }
        });

        tabbar_.activated(0);
        place.field("tabbar") << tabbar_;
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
        lab_priorityTip.caption(u8"提示:   优先级数值小 > 优先级数值大 > 默认的设置\t\t\t\t\t\t\t\t\t\t右击编辑");
        place_.field("lab_priorityTip") << lab_priorityTip;

        readConf();
    }

public:
    tab_page_alone(window wd) : tab_father(wd) {
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
    // fm.bgcolor(color_rgb(BG_COLOE));

    place place;
    place.bind(fm);
    place.div("<vert <weight=30 tabbar> <tab_frame>>");

    std::vector<std::shared_ptr<tab_father>> tabpages;
    tabbar<std::string> tabbar_{fm};

    //不知道为啥 在添加前禁用scroll 就可以实现显示最前面的scroll页面
    tabbar_.toolbox(nana::tabbar<std::string>::kits::scroll, false);

    tabbar_.push_back(u8"主要设置");
    tabpages.push_back(std::make_shared<tab_page_main>(fm));
    tabbar_.push_back(u8"全局设置");
    tabpages.push_back(std::make_shared<tab_page_overall>(fm));
    tabbar_.push_back(u8"关键词");
    tabpages.push_back(std::make_shared<tab_page_keyWord>(fm));
    tabbar_.push_back(u8"过滤名单");
    tabpages.push_back(std::make_shared<tab_page_list>(fm));
    tabbar_.push_back(u8"触发回复");
    tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm));
    tabbar_.push_back(u8"转发到群");
    tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm));
    tabbar_.push_back(u8"单独设置");
    tabpages.push_back(std::make_shared<tab_page_alone>(fm));

    std::size_t index = 0;
    for (auto& i : tabpages) {
        tabbar_.attach(index++, *i);
        place.field("tab_frame").fasten(*i);
    }
    tabbar_.toolbox(nana::tabbar<std::string>::kits::scroll, true);
    tabbar_.toolbox(nana::tabbar<std::string>::kits::list, true);

    //点击列表标签时
    tabbar_.events().click([&fm, &tabpages, &tabbar_, this]() {
        static int lastItem = 0;
        int tempItem = tabbar_.activated();

        if (!tabpages.at(lastItem)->getSave()) {
            tabbar_.activated(lastItem);

            msgbox inf{fm, u8"未保存", nana::msgbox::yes_no_cancel};
            inf << u8"是否保存已修改的内容";
            auto res = inf.show();

            if (res == nana::msgbox::pick_yes) {
                if (!tabpages.at(lastItem)->save()) return; //保存失败时
            } else if (res == nana::msgbox::pick_no) {
                tabpages.at(lastItem)->readConf();
            } else if (res == nana::msgbox::pick_cancel) {
                return;
            }

            tabbar_.activated(tempItem);
        }

        tabpages.at(tempItem)->readConf();
        lastItem = tempItem;
    });

    //窗口关闭时
    fm.events().unload([&fm, &tabpages, &tabbar_, this](const arg_unload& un) {
        int item = tabbar_.activated();

        if (!tabpages.at(item)->getSave()) {
            msgbox inf{fm, u8"未保存", nana::msgbox::yes_no_cancel};
            inf << u8"是否保存已修改的内容";
            auto res = inf.show();

            if (res == nana::msgbox::pick_yes) {
                if (!tabpages.at(item)->save()) un.cancel = true; //当保存失败时
            } else if (res == nana::msgbox::pick_no) {
                tabpages.at(item)->readConf();
            } else if (res == nana::msgbox::pick_cancel) {
                un.cancel = true;
            }
        }
    });

    tabbar_.activated(0);
    place.field("tabbar") << tabbar_;
    place.collocate();
    fm.show();
    exec();
}