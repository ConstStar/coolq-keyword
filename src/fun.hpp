#pragma once

//数据结构
#include <map>
#include <string>
#include <vector>
#include "mycq.hpp"

//系统
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <fstream>
#include <thread>

#include <boost/timer.hpp>
#include <regex>

#include "myJson.h"

using namespace std;

string appDir;
MyJson conf;

/*私聊指令*/
class Private {
    enum Private_index {
        NONE,

        SEND_GROUP_1,
        SEND_GROUP_END,
        SEND_QQ,

        ADD_KEYWORD,
        ADD_KEYWORD_WHITE,
        ADD_KEYWORD_RE,

        DEL_KEYWORD,
        DEL_KEYWORD_WHITE,
        DEL_KEYWORD_RE,

        ADD_QQLIST,
        ADD_GROUPLIST_SEND,

        DEL_QQLIST,
        DEL_GROUPLIST_SEND,

        ADD_GROUPLIST,
        DEL_GROUPLIST

    };

public:
    Private(long long fromQQ = 0) : m_fromQQ(fromQQ), m_index(NONE), m_ReplyGroup(0), m_ReplyQQ(0) {
    }

    void put_fromQQ(long long fromQQ) {
        m_fromQQ = fromQQ;
    }

    //私聊消息处理
    void funthing(const char* msg) {
        auto iter = find(conf.admin.begin(), conf.admin.end(), m_fromQQ);

        if (iter == conf.admin.end()) {
            //不是主人时将消息转发给主人
            MsgRelayFun(msg);
        } else {
            //为主人时进入指令函数
            instruct(msg);
        }
    }

    //消息转发
    void MsgRelayFun(const char* msg) {
        if (conf.relayPrivateMsg) {
            std::string SendMsg;

            //获取QQ资料
            auto QQInf = cq::get_stranger_info(m_fromQQ, true);

            //构造消息
            SendMsg = "来自" + QQInf.nickname + "(" + std::to_string(m_fromQQ) + ")的私聊消息:\n\n";
            SendMsg += msg;
            SendMsg += "\n\n(回复请发送 回复" + std::to_string(m_fromQQ) + ")";

            //发送给主人
            for (long long root : conf.admin) {
                mycq::send_private_message(root, SendMsg);
            }
        }
    }

    //指令对话
    void funcute(const char* msg) {
        switch (m_index) {
        case SEND_GROUP_1: //回复群
        {
            m_ReplyGroup = atoll(msg);
            mycq::send_private_message(m_fromQQ, "请发送回复内容");
            m_index = SEND_GROUP_END;
            break;
        }
        case SEND_GROUP_END: //回复群
        {
            char ch[100] = {'\0'};
            int t;
            t = mycq::send_group_message(m_ReplyGroup, msg);
            if (t > 0)
                sprintf(ch, "发送给QQ群%lld成功", m_ReplyGroup);
            else
                sprintf(ch, "发送给QQ群%lld失败\n错误代码：%d", m_ReplyGroup, t);
            mycq::send_private_message(m_fromQQ, ch);
            m_ReplyGroup = 0;
            m_index = NONE;
            break;
        }
        case ADD_KEYWORD: //添加关键词
        {
            //支持批量添加 解析每行数据
            unordered_set<string> temp_vector;
            OperateLine::line_get_str(msg, temp_vector);

            for (auto temp_keyword : temp_vector) {
                WKEYWORD temp_add(temp_keyword);
                conf.alone[0].keyWord.insert(temp_add);
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加 默认关键词 成功");

            m_index = NONE;
            break;
        }
        case DEL_KEYWORD: //删除关键词
        {
            bool del_bool = false;

            auto it = conf.alone[0].keyWord.begin();

            while (it != conf.alone[0].keyWord.end()) {
                if (it->keyWord == msg) {
                    auto del = it;

                    it = conf.alone[0].keyWord.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除 默认关键词 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除 默认关键词 失败");

            m_index = NONE;
            break;
        }
        case ADD_QQLIST: //添加白名单/监控名单
        {
            //支持批量添加 解析每行数据
            unordered_set<long long> temp_vector;
            OperateLine::line_get_ll(msg, temp_vector);

            for (auto temp_longlong : temp_vector) {
                conf.alone[0].QQList.insert(temp_longlong);
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加默认 白名单/监控名单 成功");

            m_index = NONE;
            break;
        }
        case DEL_QQLIST: //删除白名单/监控名单
        {
            bool del_bool = false;

            long long del_QQId = 0;
            del_QQId = atoll(msg);

            if (del_QQId == 0) {
                mycq::send_private_message(m_fromQQ, "格式有误");
                m_index = NONE;
                break;
            }

            auto it = conf.alone[0].QQList.begin();

            while (it != conf.alone[0].QQList.end()) {
                if (*it == del_QQId) {
                    auto del = it;

                    it = conf.alone[0].QQList.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除默认 白名单/监控名单 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除默认 白名单/监控名单 失败");

            m_index = NONE;
            break;
        }
        case ADD_GROUPLIST: //添加监控群
        {
            //支持批量添加 解析每行数据
            unordered_set<long long> temp_vector;
            OperateLine::line_get_ll(msg, temp_vector);

            for (auto temp_longlong : temp_vector) {
                conf.alone[0].groupList.insert(temp_longlong);
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加 默认监控群 成功");
            m_index = NONE;
            break;
        }
        case DEL_GROUPLIST: //删除监控群
        {
            bool del_bool = false;

            long long del_GourpId = 0;
            del_GourpId = atoll(msg);

            if (del_GourpId == 0) {
                mycq::send_private_message(m_fromQQ, "格式有误");
                m_index = NONE;
                break;
            }

            auto it = conf.alone[0].groupList.begin();

            while (it != conf.alone[0].groupList.end()) {
                if (*it == del_GourpId) {
                    auto del = it;

                    it = conf.alone[0].groupList.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除 默认监控群 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除 默认监控群 失败");
            m_index = NONE;
            break;
        }
        case SEND_QQ: //回复QQ
        {
            char buf[100] = {'\0'};
            int t;
            t = mycq::send_private_message(m_ReplyQQ, msg);
            if (t > 0)
                sprintf(buf, "发送给 QQ%lld 成功", m_ReplyQQ);
            else
                sprintf(buf, "发送给 QQ%lld 失败\n错误代码：%d", m_ReplyQQ, t);
            mycq::send_private_message(m_fromQQ, buf);
            m_ReplyQQ = 0;
            m_index = NONE;
            break;
        } //回复QQ

        case ADD_KEYWORD_WHITE: //添加白名单关键词
        {
            //支持批量添加 解析每行数据
            unordered_set<string> temp_vector;
            OperateLine::line_get_str(msg, temp_vector);

            for (auto temp_keyword : temp_vector) {
                WKEYWORD temp_add(temp_keyword);
                conf.alone[0].keyWordWhite.insert(temp_add);
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加默认 白名单关键词 成功");

            m_index = NONE;
            break;
        }
        case DEL_KEYWORD_WHITE: //删除白名单关键词
        {
            bool del_bool = false;

            auto it = conf.alone[0].keyWordWhite.begin();

            while (it != conf.alone[0].keyWordWhite.end()) {
                if (it->keyWord == msg) {
                    auto del = it;

                    it = conf.alone[0].keyWordWhite.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }
            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除默认 白名单关键词 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除默认 白名单关键词 失败");

            m_index = NONE;
            break;
        }

        case ADD_KEYWORD_RE: //添加正则表达式关键词
        {
            //支持批量添加 解析每行数据
            unordered_set<string> temp_vector;
            OperateLine::line_get_str(msg, temp_vector);

            for (auto temp_keyword : temp_vector) {
                WKEYWORD temp_add(temp_keyword);
                conf.alone[0].keyWordRegex.insert(temp_add);
            }
            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加默认 正则表达式关键词 成功");

            m_index = NONE;
            break;
        }
        case DEL_KEYWORD_RE: //删除正则表达式关键词
        {
            bool del_bool = false;

            auto it = conf.alone[0].keyWordRegex.begin();

            while (it != conf.alone[0].keyWordRegex.end()) {
                if (it->keyWord == msg) {
                    auto del = it;

                    it = conf.alone[0].keyWordRegex.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }
            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除默认 正则表达式关键词 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除默认 正则表达式关键词 失败");

            m_index = NONE;
            break;
        }

        case ADD_GROUPLIST_SEND: //添加转发到群号码
        {
            //支持批量添加 解析每行数据
            unordered_set<long long> temp_vector;
            OperateLine::line_get_ll(msg, temp_vector);

            for (auto temp_longlong : temp_vector) {
                conf.alone[0].relayGroupList.insert(temp_longlong);
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            mycq::send_private_message(m_fromQQ, "添加默认 转发群 成功");

            m_index = NONE;
            break;
        }
        case DEL_GROUPLIST_SEND: //删除转发到群号码
        {
            bool del_bool = false;

            long long del_QQId = 0;
            del_QQId = atoll(msg);

            if (del_QQId == 0) {
                mycq::send_private_message(m_fromQQ, "格式有误");
                m_index = NONE;
                break;
            }

            auto it = conf.alone[0].relayGroupList.begin();

            while (it != conf.alone[0].relayGroupList.end()) {
                if (*it == del_QQId) {
                    auto del = it;

                    it = conf.alone[0].relayGroupList.erase(del);
                    del_bool = true;
                } else {
                    it++;
                }
            }

            //重新构造json 并写入文件
            conf.all2json();
            conf.json2file();

            if (del_bool)
                mycq::send_private_message(m_fromQQ, "删除默认 转发群 成功");
            else
                mycq::send_private_message(m_fromQQ, "删除默认 转发群 失败");

            m_index = NONE;
            break;
        }
        }
    }

    //指令检测
    int instruct(const char* msg) {
        string& prefix = conf.prefix;

        //检测是否有为处理完的指令对话
        if (m_index != NONE) {
            funcute(msg);
            return 0;
        }

        //功能菜单
        if (!std::string(prefix + "功能").compare(msg) || !std::string(prefix + "菜单").compare(msg)
            || !strcmp(msg, "关键词触发器菜单")) {
            std::string menu =
                "**设置类(自定义需加前缀)**\n"
                "[CQ:emoji,id=9999]查看/添加/删除关键词\n"
                "[CQ:emoji,id=9999]查看/添加/删除白名单关键词\n"
                "[CQ:emoji,id=9999]查看/添加/删除正则表达式关键词\n"
                "[CQ:emoji,id=127984]查看/添加/删除监控群\n"
                "[CQ:emoji,id=127984]查看/添加/删除转发群\n"
                "[CQ:emoji,id=128203]查看/添加/删除监控名单\n"
                "[CQ:emoji,id=128203]查看/添加/删除白名单\n"

                "\n**其他类(自定义需加前缀)**\n"
                "[CQ:emoji,id=127380]回复群+QQ群号\n"
                "[CQ:emoji,id=127380]回复+QQ号\n"
                //"[CQ:emoji,id=127380]退出群+QQ群号\n"
                "\n请发送功能名称（不包括表情）\n"
                "如: 查看关键词";

            mycq::send_private_message(m_fromQQ, menu);
            m_index = NONE;
        }

        //回复类
        else if (!std::string(prefix + "回复群").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送要回复的QQ群号");
            m_index = SEND_GROUP_1;
        } else if (std::string(msg).find(prefix + "回复群") != string::npos) {
            m_ReplyGroup = atoll(msg + std::string(prefix + "回复群").length());

            if (m_ReplyGroup == 0) {
                sscanf(msg, "回复群＋%lld", m_ReplyGroup);
                if (m_ReplyGroup == 0) {
                    mycq::send_private_message(m_fromQQ, "输入有误，请按照格式重新发送");
                    return -1;
                }
            }
            mycq::send_private_message(m_fromQQ, "请发送回复内容");
            m_index = SEND_GROUP_END;
        } else if (std::string(msg).find(prefix + "回复") != string::npos) {
            m_ReplyQQ = atoll(msg + std::string(prefix + "回复").length());
            if (m_ReplyQQ != 0) {
                mycq::send_private_message(m_fromQQ, "请发送要回复的消息");
                m_index = SEND_QQ;
            } else {
                mycq::send_private_message(m_fromQQ, "输入有误，请发送 回复+QQ号");
                m_index = NONE;
            }

        }

        //默认关键词
        else if (!std::string(prefix + "查看关键词").compare(msg)) {
            string SendMsg = "全局默认 关键词\n";

            for (auto KeyWord : conf.alone[0].keyWord) SendMsg += KeyWord.keyWord + "\n";

            mycq::send_private_message(m_fromQQ, SendMsg);
            m_index = NONE;
        } else if (!std::string(prefix + "添加关键词").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送添加的关键词(可批量添加，每行一个)");
            m_index = ADD_KEYWORD;
        } else if (!std::string(prefix + "删除关键词").compare(msg)) {
            std::string SendMsg = "全局默认 关键词：\n";

            for (auto WKeyWord : conf.alone[0].keyWord) {
                SendMsg += WKeyWord.keyWord + "\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            mycq::send_private_message(m_fromQQ, "请发送准备删除的关键词");
            m_index = DEL_KEYWORD;
        }

        //白名单关键词
        else if (!std::string(prefix + "查看白名单关键词").compare(msg)) {
            string SendMsg = "全局默认 白名单关键词：\n";

            for (auto KeyWord : conf.alone[0].keyWordWhite) SendMsg += KeyWord.keyWord + "\n";

            mycq::send_private_message(m_fromQQ, SendMsg);
            m_index = NONE;
        } else if (!std::string(prefix + "添加白名单关键词").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送添加的 白名单关键词 (可批量添加，每行一个)");
            m_index = ADD_KEYWORD_WHITE;
        } else if (!std::string(prefix + "删除白名单关键词").compare(msg)) {
            std::string SendMsg = "全局默认 白名单关键词：\n";

            for (auto WKeyWord : conf.alone[0].keyWordWhite) {
                SendMsg += WKeyWord.keyWord + "\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            mycq::send_private_message(m_fromQQ, "请发送准备删除的 白名单关键词");
            m_index = DEL_KEYWORD_WHITE;
        }

        //正则表达式关键词
        else if (!std::string(prefix + "查看正则表达式关键词").compare(msg)) {
            string SendMsg = "全局默认 正则表达式关键词：\n";

            for (auto KeyWord : conf.alone[0].keyWordRegex) SendMsg += KeyWord.keyWord + "\n";

            mycq::send_private_message(m_fromQQ, SendMsg);
            m_index = NONE;
        } else if (!std::string(prefix + "添加正则表达式关键词").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送添加的 正则表达式关键词 (可批量添加，每行一个)");
            m_index = ADD_KEYWORD_RE;
        } else if (!std::string(prefix + "删除正则表达式关键词").compare(msg)) {
            std::string SendMsg = "全局默认 正则表达式关键词：\n";

            for (auto WKeyWord : conf.alone[0].keyWordRegex) {
                SendMsg += WKeyWord.keyWord + "\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            mycq::send_private_message(m_fromQQ, "请发送准备删除的 正则表达式关键词");
            m_index = DEL_KEYWORD_RE;
        }

        //设置转发群
        else if (!std::string(prefix + "查看转发群").compare(msg)) {
            std::string SendMsg = "全局默认 转发群：\n";
            auto temp_GourpList = cq::get_group_list();

            //列出监控群名单
            for (auto id : conf.alone[0].relayGroupList) {
                string groupName;
                for (auto& group : temp_GourpList) {
                    if (group.group_id == id) {
                        groupName = group.group_name;
                        break;
                    }
                }
                SendMsg += groupName + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            m_index = NONE;
        } else if (!std::string(prefix + "添加转发群").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送添加的 群号码 (可批量添加，每行一个)");
            m_index = ADD_GROUPLIST_SEND;
        } else if (!std::string(prefix + "删除转发群").compare(msg)) {
            std::string SendMsg = "全局默认 转发群：\n";
            auto temp_GourpList = cq::get_group_list();

            //列出监控群名单
            for (auto id : conf.alone[0].relayGroupList) {
                string groupName;
                for (auto& group : temp_GourpList) {
                    if (group.group_id == id) {
                        groupName = group.group_name;
                        break;
                    }
                }
                SendMsg += groupName + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);
            mycq::send_private_message(m_fromQQ, "请发送要删除 群号码");
            m_index = DEL_GROUPLIST_SEND;
        }

        //设置监控群
        else if (!std::string(prefix + "添加监控群").compare(msg)) {
            mycq::send_private_message(m_fromQQ, "请发送添加的群号码(可批量添加，每行一个)");
            m_index = ADD_GROUPLIST;
        } else if (!std::string(prefix + "删除监控群").compare(msg)) {
            std::string SendMsg = "全局默认 监控群：\n";
            auto temp_GourpList = cq::get_group_list();

            //列出监控群名单
            for (auto id : conf.alone[0].groupList) {
                string groupName;
                for (auto group : temp_GourpList) {
                    if (group.group_id == id) {
                        groupName = group.group_name;
                        break;
                    }
                }

                SendMsg += groupName + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);
            mycq::send_private_message(m_fromQQ, "请发送要删除群号码");
            m_index = DEL_GROUPLIST;
        } else if (!std::string(prefix + "查看监控群").compare(msg)) {
            std::string SendMsg = "全局默认 监控群：\n";
            auto temp_GourpList = cq::get_group_list();

            //列出监控群名单
            for (auto id : conf.alone[0].groupList) {
                string groupName;
                for (auto group : temp_GourpList) {
                    if (group.group_id == id) {
                        groupName = group.group_name;
                        break;
                    }
                }
                SendMsg += groupName + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            m_index = NONE;
        }

        //设置监控名单
        else if (!std::string(prefix + "添加监控名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为白名单，与您的命令不符，请在先界面中设置为监控名单模式");
                return 0;
            }

            mycq::send_private_message(m_fromQQ, "请发送添加的白名单/监控名单QQ(可批量添加，每行一个)");
            m_index = ADD_QQLIST;
        } else if (!std::string(prefix + "删除监控名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为白名单，与您的命令不符，请在先界面中设置为监控名单模式");
                return 0;
            }
            std::string SendMsg = "全局默认 监控名单：\n";

            for (auto id : conf.alone[0].QQList) {
                auto QQinf = cq::get_stranger_info(id);
                SendMsg += QQinf.nickname + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);
            mycq::send_private_message(m_fromQQ, "请发送要删除白名单号码");
            m_index = DEL_QQLIST;
        } else if (!std::string(prefix + "查看监控名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为白名单，与您的命令不符，请在先界面中设置为监控名单模式");
                return 0;
            }
            std::string SendMsg = "全局默认 监控名单：\n";

            for (auto temp : conf.alone[0].QQList) {
                auto QQinf = cq::get_stranger_info(temp);
                SendMsg += QQinf.nickname + "(" + to_string(temp) + ")\n";
            }
        }
        //设置白名单
        else if (!std::string(prefix + "添加白名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为监控名单，与您的命令不符，请在先界面中设置为白名单模式");
                return 0;
            }

            mycq::send_private_message(m_fromQQ, "请发送添加的白名单/监控名单QQ(可批量添加，每行一个)");
            m_index = ADD_QQLIST;
        } else if (!std::string(prefix + "删除白名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为监控名单，与您的命令不符，请在先界面中设置为白名单模式");
                return 0;
            }
            std::string SendMsg = "全局默认 白名单：\n";

            for (auto id : conf.alone[0].QQList) {
                auto QQinf = cq::get_stranger_info(id);
                SendMsg += QQinf.nickname + "(" + to_string(id) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);
            mycq::send_private_message(m_fromQQ, "请发送要删除白名单号码");
            m_index = DEL_QQLIST;
        } else if (!std::string(prefix + "查看白名单").compare(msg)) {
            if (conf.alone[0].QQListType == 0) {
                mycq::send_private_message(m_fromQQ,
                                           "当前特殊名单模式为监控名单，与您的命令不符，请在先界面中设置为白名单模式");
                return 0;
            }
            std::string SendMsg = "全局默认 白名单：\n";

            for (auto temp : conf.alone[0].QQList) {
                auto QQinf = cq::get_stranger_info(temp);
                SendMsg += QQinf.nickname + "(" + to_string(temp) + ")\n";
            }

            mycq::send_private_message(m_fromQQ, SendMsg);

            m_index = NONE;
        }

        return m_index;
    }

private:
    Private_index m_index; //指令位置

    long long m_ReplyGroup; //需要回复的群号码
    long long m_ReplyQQ; //需要回复的QQ号码
    long long m_fromQQ; //消息来自的QQ号码
};

/*消息处理*/

class OperateMsg {
public:
    //处理
    void dealFun(int conf_index) {
        int& dealType = conf.alone[conf_index].dealType;

        //撤回
        if (conf.alone[conf_index].deleteMsg) mycq::delete_message(m_msgId);

        if (dealType == 1) {
            if (evet.is_anonymous())
                mycq::set_group_anonymous_ban(m_fromGroup, m_fromAnonymous, conf.alone[conf_index].banTimeLen * 60);
            else
                mycq::set_group_ban(m_fromGroup, m_fromQQ, conf.alone[conf_index].banTimeLen * 60);

            std::string TimeStr;

            int TimeDay = 0;
            int TimeHour = 0;
            int TimeMin = 0;

            TimeDay = conf.alone[conf_index].banTimeLen / 1440;
            TimeHour = (conf.alone[conf_index].banTimeLen - TimeDay * 1440) / 60;
            TimeMin = conf.alone[conf_index].banTimeLen % 60;

            if (TimeDay) {
                TimeStr = std::to_string(TimeDay) + "天";
            }

            if (TimeHour) {
                TimeStr += std::to_string(TimeHour) + "小时";
            }

            if (TimeMin) {
                TimeStr += std::to_string(TimeMin) + "分钟";
            }

            dealTypeStr = "禁言";
            dealTypeStr += TimeStr;

        } else if (dealType == 2) {
            mycq::set_group_kick(m_fromGroup, m_fromQQ, false);

            dealTypeStr = "踢出";

        } else if (dealType == 3) {
            mycq::set_group_kick(m_fromGroup, m_fromQQ, true);

            dealTypeStr = "踢出并拉黑";

        } else {
            dealTypeStr = "未做处理";
        }
    }

    //构造提醒主人消息
    string keyWordSendAdminFun(int conf_index) {
        stringstream msg;

        if (m_fromQQ == 80000000) {
            msg << "来自群" << m_fromGroup << +"中\n";
            msg << "QQ号码:" << m_fromQQ << "(QQ匿名)"
                << "\n";
            msg << "QQ名称:"
                << "匿名消息"
                << "\n";
            msg << "QQ群名片:"
                << "匿名消息"
                << "\n";
            msg << "由于内容:\n\n" << m_msg << "\n";
        } else {
            auto QQInf = cq::get_group_member_info(m_fromGroup, m_fromQQ);

            msg << "来自群" << m_fromGroup << +"中\n";
            msg << "QQ号码:" << m_fromQQ << "\n";
            msg << "QQ名称:" << QQInf.nickname << "\n";
            msg << "QQ群名片:" << QQInf.card << "\n";
            msg << "由于内容:\n\n" << m_msg << "\n";
        }

        if (!keyWordRegex.empty()) {
            msg << "\n正则表达式:" << keyWordRegex << "\n";
        }

        msg << "\n触发了关键词:" << keyWord;
        msg << "\n处理方式:" << dealTypeStr;
        msg << "\n本次处理总耗时:";
        msg << m_time.elapsed() << "秒";
        msg << "\n(回复请发送:回复群" << m_fromGroup << ")";

        return msg.str();
    }

    //触发关键词操作
    void KeyWrodWarn(int conf_index) {
        //处理
        dealFun(conf_index);

        //转发到群
        string relayGroupMsg(conf.alone[conf_index].relayGroupWord);
        //变量
        relayGroupMsgWord(relayGroupMsg, conf_index);
        //发送
        for (long long GroupId : conf.alone[conf_index].relayGroupList) {
            mycq::send_group_message(GroupId, relayGroupMsg);
        }

        //群内警告（回复群内容消息）
        if (conf.alone[conf_index].keyWordGroupWarn) //群内回复是否开启
        {
            string sendMsg(conf.alone[conf_index].keyWordGroupWarnWord);
            if (conf.alone[conf_index].keyWordGroupWarnWord.empty()) //自定义内容为空时使用默认提示
            {
                sendMsg = "{at} 触发关键词，处理方式:{处理方式}";
            }
            KeyWordWarnMsg(sendMsg, conf_index); //变量
            mycq::send_group_message(m_fromGroup, sendMsg);
        }

        //触发后回复私聊消息
        if (conf.alone[conf_index].keyWordPrivateWarn) {
            if (!conf.alone[conf_index].keyWordPrivateWarnWord.empty()) {
                string sendMsg(conf.alone[conf_index].keyWordPrivateWarnWord);
                //解析变量
                KeyWordWarnMsg(sendMsg, conf_index);
                mycq::send_private_message(m_fromQQ, sendMsg);
            }
        }

        //提醒主人
        if (conf.alone[conf_index].keyWordSendAdmin) //触发后提醒主人是否开启
        {
            string sendAdminMsg = keyWordSendAdminFun(conf_index);
            for (long long root : conf.admin) {
                mycq::send_private_message(root, sendAdminMsg);
            }
        }
    }

    //删除cq码
    std::wstring DelCQ(std::wstring msg) {
        if (msg.find(L"[CQ:") != wstring::npos) {
            wregex e1(L"\\[CQ:.*\\]");
            msg = regex_replace(msg, e1, L"");
        }

        return msg;
    }

    //转发到群格式
    void relayGroupMsgWord(std::string& str, int conf_index) {
        //默认内容为msg
        if (str.empty()) {
            str = "{msg}";
        }

        //消息内容
        if (str.find("{msg}") != string::npos) {
            string temp_msg(m_msg);
            int frontLine = 0;
            int afterLine = temp_msg.size();

            int i = 0;

            if (conf.alone[conf_index].relayGroupMsg_trimFront) {
                for (auto it = temp_msg.begin(); it != temp_msg.end(); it++) {
                    frontLine++;
                    if (*it == '\n') {
                        i++;
                        if (i == conf.alone[conf_index].relayGroupMsg_trimFront) break;
                    }
                }
            }

            if (conf.alone[conf_index].relayGroupMsg_trimBack) {
                i = 0;
                for (auto it = temp_msg.rbegin(); it != temp_msg.rend(); it++) {
                    afterLine--;
                    if (*it == '\n') {
                        i++;
                        if (i == conf.alone[conf_index].relayGroupMsg_trimBack) break;
                    }
                }
            }

            temp_msg[afterLine] = 0;
            temp_msg = temp_msg.c_str() + frontLine;

            //检测消息格式是否错误
            if (temp_msg.empty()) {
                string sendMsg;
                sendMsg = "解析后消息格式错误\n消息内容:\n\n";
                sendMsg += m_msg;
                sendMsg += "\n\n请查看转发到群格式中删除行是否有误";

                for (auto temp : conf.admin) {
                    mycq::send_private_message(temp, sendMsg);
                }
                str = "";

                return;
            }

            str = OperateStr::replace_all_distinct(str, "{msg}", temp_msg);
        }

        //获取当前时间
        SYSTEMTIME sys;
        GetLocalTime(&sys);

        char date[20];
        char time[20];
        char Week[20];
        char weekStr[8][4] = {"天", "一", "二", "三", "四", "五", "六"};

        sprintf(date, "%4d年%02d月%02d日", sys.wYear, sys.wMonth, sys.wDay);
        sprintf(time, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
        sprintf(Week, "星期%s", weekStr[sys.wDayOfWeek]);

        //日期
        str = OperateStr::replace_all_distinct(str, "{日期}", date);

        //时间
        str = OperateStr::replace_all_distinct(str, "{时间}", time);

        //星期
        str = OperateStr::replace_all_distinct(str, "{星期}", Week);

        //触发的关键词
        str = OperateStr::replace_all_distinct(str, "{关键词}", keyWord);

        //触发的QQ号码
        str = OperateStr::replace_all_distinct(str, "{QQ号码}", to_string(m_fromQQ));

        auto QQInf = cq::get_group_member_info(m_fromGroup, m_fromQQ, true);
        auto GroupList = cq::get_group_list();
        cq::Group group;
        //触发的群名称
        for (auto& temp : GroupList) {
            group = temp;
        }

        //触发的QQ名称
        str = OperateStr::replace_all_distinct(str, "{QQ名称}", QQInf.nickname);

        //触发的QQ名片
        str = OperateStr::replace_all_distinct(str, "{QQ名片}", QQInf.card);

        str = OperateStr::replace_all_distinct(str, "{群号码}", to_string(group.group_id));
        str = OperateStr::replace_all_distinct(str, "{群名称}", group.group_name);
    }

    //自定义触发关键词提醒
    void KeyWordWarnMsg(std::string& str, int conf_index) {
        //艾特
        str = OperateStr::replace_all_distinct(str, "{at}", "[CQ:at,qq=" + std::to_string(m_fromQQ) + "]");

        //获取当前时间
        SYSTEMTIME sys;
        GetLocalTime(&sys);

        char date[20];
        char time[20];
        char Week[20];
        char weekStr[8][4] = {"天", "一", "二", "三", "四", "五", "六"};

        sprintf(date, "%4d年%02d月%02d日", sys.wYear, sys.wMonth, sys.wDay);
        sprintf(time, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
        sprintf(Week, "星期%s", weekStr[sys.wDayOfWeek]);

        //日期
        str = OperateStr::replace_all_distinct(str, "{日期}", date);

        //时间
        str = OperateStr::replace_all_distinct(str, "{时间}", time);

        //星期
        str = OperateStr::replace_all_distinct(str, "{星期}", Week);

        //处理方式
        str = OperateStr::replace_all_distinct(str, "{处理方式}", dealTypeStr);

        //触发的关键词
        str = OperateStr::replace_all_distinct(str, "{关键词}", keyWord);

        //触发的QQ号码
        str = OperateStr::replace_all_distinct(str, "{QQ号码}", to_string(m_fromQQ));

        auto QQInf = cq::get_group_member_info(m_fromGroup, m_fromQQ, true);
        auto GroupList = cq::get_group_list();
        cq::Group group;

        //触发的群名称
        for (auto& temp : GroupList) {
            group = temp;
        }

        //触发的QQ名称
        str = OperateStr::replace_all_distinct(str, "{QQ名称}", QQInf.nickname);

        //触发的QQ名片
        str = OperateStr::replace_all_distinct(str, "{QQ名片}", QQInf.card);

        //触发的群号码
        str = OperateStr::replace_all_distinct(str, "{群号码}", to_string(group.group_id));

        //触发的群名称
        str = OperateStr::replace_all_distinct(str, "{群名称}", group.group_name);
    }

    //白名单关键词检测
    bool KeyWordWhiteFun(int conf_index) {
        if (conf.alone[conf_index].keyWordWhite.empty()) return false;

        wstring wmsg;
        if (conf.alone[conf_index].deleteCQCode) {
            wmsg = m_wmsg_delCQ;
        } else {
            wmsg = m_wmsg;
        }

        //查找关键词 如果存在某一个 则返回true
        for (WKEYWORD KeyWord_one : conf.alone[conf_index].keyWordWhite) {
            if (wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos) {
                return true;
            }
        }

        return false;
    }

    //关键词对比  KeyWord关键词容器 Streng强力检测 KeyWordWarn返回触发的关键词
    bool KeyWordFun(int conf_index) {
        //判断关键词是否为空
        if (conf.alone[conf_index].keyWord.empty()) {
            return false;
        }

        wstring wmsg;
        if (conf.alone[conf_index].deleteCQCode) {
            wmsg = m_wmsg_delCQ;
        } else {
            wmsg = m_wmsg;
        }

        //普通关键词检测
        for (auto KeyWord_one : conf.alone[conf_index].keyWord) {
            if (wmsg.find(KeyWord_one.wkeyWrod) != std::wstring::npos) {
                //记录触发的关键词
                keyWord = KeyWord_one.keyWord;

                return true;
            }
        }

        //强力检测
        if (conf.alone[conf_index].streng) {
            for (auto KeyWord_one : conf.alone[conf_index].keyWord) {
                //记录关键词字数与匹配数
                int temp_num = KeyWord_one.wkeyWrod.size(), temp_num_find = 0;

                //判断消息和关键词是否为空
                if (KeyWord_one.wkeyWrod.empty() || m_wmsg.empty()) {
                    return false;
                }

                //开始强力检测   逐个字对比
                for (auto keyc : KeyWord_one.wkeyWrod) {
                    for (auto msgc : wmsg) {
                        if (keyc == msgc) {
                            temp_num_find++;
                            break;
                        }
                    }
                }

                //触发强力检测后
                if (temp_num_find >= temp_num) {
                    //记录触发的关键词
                    keyWord = KeyWord_one.keyWord;

                    return true;
                }
            }
        }

        return false;
    }

    //正则表达式关键词检测
    bool KeyKordRegexFun(int conf_index) {
        wstring wmsg;
        if (conf.alone[conf_index].deleteCQCode) {
            wmsg = m_wmsg_delCQ;
        } else {
            wmsg = m_wmsg;
        }

        for (WKEYWORD aloneRegex : conf.alone[conf_index].keyWordRegex) {
            try {
                wregex re(aloneRegex.wkeyWrod);
                wsmatch RE;

                bool rec = regex_search(wmsg, RE, re);

                if (rec) {
                    keyWord = OperateStr::wstring2string(RE.str());
                    keyWordRegex = aloneRegex.keyWord;
                    return true;
                }

            } catch (exception& e) {
                string SendMsg;
                SendMsg += "正则表达式崩溃\n";
                SendMsg += "表达式:";
                SendMsg += aloneRegex.keyWord;
                SendMsg += "消息:\n\n";
                SendMsg += m_msg;
                SendMsg += "exception返回的错误消息:";
                SendMsg += e.what();

                for (long long root : conf.admin) {
                    mycq::send_private_message(root, SendMsg);
                }
            }
        }
        return false;
    }

    //内容处理功能  触发true  未触发false
    bool indexFun(int conf_index) {
        //查找是否为默认监控群
        auto list_bool =
            find(conf.alone[conf_index].groupList.begin(), conf.alone[conf_index].groupList.end(), m_fromGroup);

        //如果找到看 为默认监控群
        if (list_bool == conf.alone[conf_index].groupList.end()) return false;

        //如果为检测白名单模式
        if (conf.alone[conf_index].QQListType == 0) {
            //检测是否为白名单 如果为白名单将退出函数
            if (find(conf.alone[conf_index].QQList.begin(), conf.alone[conf_index].QQList.end(), m_fromQQ)
                != conf.alone[conf_index].QQList.end()) {
                return false;
            }
        }
        //如果为检测监控名单模式
        else if (conf.alone[conf_index].QQListType == 1) {
            //如果在监控名单没找到此号码 则退出函数
            if (find(conf.alone[conf_index].QQList.begin(), conf.alone[conf_index].QQList.end(), m_fromQQ)
                == conf.alone[conf_index].QQList.end()) {
                return false;
            }
        }

        //如果关键词为白名单 将退出函数
        if (KeyWordWhiteFun(conf_index)) {
            return false;
        }

        //如果触发了关键词
        if (KeyWordFun(conf_index)) {
            KeyWrodWarn(conf_index);
            return true;
        }

        //如果触发了正则表达式关键词
        if (KeyKordRegexFun(conf_index)) {
            KeyWrodWarn(conf_index);
            return true;
        }

        return false;
    }

    //内容处理
    void MsgFun() {
        m_time.restart();
        for (auto temp : conf.alone) {
            //全局设置最后检测
            if (temp.first == 0) continue;

            if (temp.second.use) {
                bool ret = indexFun(temp.first);

                if (ret) return;
            }
        }

        indexFun(0);
    }

    OperateMsg(cq::GroupMessageEvent evet) : evet(evet) {
        m_fromQQ = evet.user_id;
        m_fromGroup = evet.group_id;
        m_fromAnonymous = evet.anonymous;
        m_msg = evet.message;
        m_msgId = evet.message_id;
        //将消息的宽字符串格式存放到对象中
        m_wmsg = OperateStr::string2wstring(evet.message);

        m_wmsg_delCQ = DelCQ(m_wmsg);
    }

private:
    int64_t m_fromQQ; //来自的QQ号码
    int64_t m_fromGroup; //来自的群号
    cq::Anonymous m_fromAnonymous; //来源的匿名信息
    string m_msg; //消息内容
    int64_t m_msgId; //消息id
    boost::timer m_time; //计算程序流失的时间

    wstring m_wmsg; //宽字节消息内容
    wstring m_wmsg_delCQ; //删除CQ码
    cq::GroupMessageEvent& evet;

    //触发后内容
    string dealTypeStr; //处理方式
    string keyWord; //普通关键词
    string keyWordRegex; //正则表达式关键词
};