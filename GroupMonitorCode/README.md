## 前言  
本项目由VS2017编写,如果您打算直接在本SDK项目的基础上编写,请确认兼容性。  
您也可以新建一个项目,请参照下一节内容.  

## 本SDK特点  
- 以 CQP SDK V9.16 为基础开发  
- 超棒的事件包装机制  
- 超棒的API包装机制  
- 超棒的工具集合
  
# 插件编写快速开始  
> - 本教程默认读者已经知晓C++基础知识以及将C++编译成DLL部分的知识  
> - **本教程随时有可能过时,请以官方易语言SDK为准**  
  
## SDK目录结构说明  

根目录说明

|目录          |说明|
|:-            |:-|
|ExampleApp.sln|示例工程|
|src/          |示例工程源码|
|lib/          |包含sdk所需的两个lib文件|
|inculde/      |CQSDK头文件目录|

然后说说头文件目录里的文件都是什么作用

|目录          |说明|
|:-            |:-|
|CQAPI_EX      |酷q功能调用，例如群消息发送，获取群列表，禁言等|
|CQEVE         |事件宏，用于快速建立事件接受函数，例如接受酷q启动事件|
|CQEVE_*       |以CQEVE_开头的文件也是事件宏，但是更加好用，请看示例|
|CQEVE_ALL     |包含所有事件实现宏，用于方便输入，无需包含多个CQEVE_头文件|
|CQLogger      |酷q日志工具，请看示例|
|CQMsgCode     |酷q消息处理工具以及酷q码生成方法，请看示例|
|INItool       |配置文件工具，请看示例|
|winSpeak      |非常厉害的语音生成工具，但是推荐使用酷q码的形式，请看示例|

## 准备编译环境
这一节为那些无法打开sln或者不是使用vs2017的人员准备  

1. 首先,强烈推荐将`\src\CQSDK`目录加入头文件搜索列表  
在vs2017里: `项目` - `属性` - `vc++目录` - `包含目录`  
如果不知道如何添加,您可以直接将此目录所有头文件复制到您的项目文件夹

2. 推荐将`appmain.cpp`和`APPINFO.h`加入到您的项目中,作为入口文件.  
`appmain.cpp`中有一些示例和部分必须函数
`APPINFO.h`则包含了插件的id和其他信息
`main.cpp`额外的dll主文件,如果您建立的是空项目可以使用这个

3. 包含SDK必须的`SDK_*.lib`,可参照`APPINFO.h`第1到5行  
如果您无法包含此lib或者需要定制自己的SDK,您可以删除`APPINFO.h`第1到5行  
并将`\src\CQSDKCPP`目录复制到您的项目  
务必保持和原来一样的目录结构,否则.cpp文件找不到对应的.h文件  

4. 关闭启用C++异常
项目-->XXX属性--->配置属性--->C/C++--->代码生成-->右边--->启用C++异常--->改成：否

至此您可以尝试第一次编译. 
如有问题,请带上报错和您的项目,一起反馈给我.

## 新项目  
在开始写插件之前, 首先要决定**插件id**(查看[规范](http://d.cqp.me/Pro/%E5%BC%80%E5%8F%91/%E5%9F%BA%E7%A1%80%E4%BF%A1%E6%81%AF#appid "http://d.cqp.me/Pro/Pro/开发/基础信息#appid"))  
- 例如 me.cqp.coxxs.weather  
  
**注意:**  
1. 插件id不是插件名称.  
2. 这个id必须独一无二且仅用于这个插件.  
  
**说明:**  
- 插件id可以保证即使有人开发了和您一样的插件,也不会被人或者程序认错  
  
决定完毕之后可以开始写一个漂亮的插件了  
#### **如无特殊说明,将用[应用ID]代指您取名的插件id.**  
  
## JSON  
一个插件,由两部分组成  
1. JSON文件:此文件用于描述插件功能.  
2. dll文件 :此文件是插件的本体.  
  
先来说说JSON文件。  
JSON文件是存储插件基本信息的文本文件,其数据存储格式为JSON。  
以下是关于酷Q插件JSON文件的示例。  
它包含了最基本的插件信息。  
```json  
{  
    "ret"        : 1,
    "apiver"     : 9,
    "name"       : "应用名称",
    "version"    : "版本号",    //文字版版本号,可以加上beta或者pro什么的  
    "version_id" : 1,        //版本号,每次发布必须+1  
    "author"     : "作者",
    "description": "引用描述",
    "event"      : [],
    "menu"       : [],
    "status"     : [],
    "auth"       : []
}  
```  
如果您要使用这份JSON。  
请按照实际情况修改 `name`, `version`, `version_id`, `author`, `description`字段。  
最后将文件名改为 **[应用ID].JSON** 并保存到您的项目文件夹备用.  
  
## DLL  
酷Q要求有指定函数`AppInfo`用于返回应用ID。  
这也是唯一的硬性要求。  
您可以在您的代码加入以下代码快速实现。  
  
```cpp  
#define CQAPPID "[应用ID]"  
  
#include "CQEVE.h"//位于SDK源码 src/CQSDK 目录下  
MUST_AppInfo_RETURN(CQAPPID)  
```  
> 关于这个函数的更多说明可以参阅详细说明书(暂无)  
  
现在将其编译为`[应用ID].DLL`,然后和JSON文件一起放入开启了开发者模式的酷Q的APP目录。  
启动酷Q后就能在插件管理器看到这款插件了.  
  
### 开发模式  
>虽然这并不是本教程的范畴,姑且从官方网站复制一份  
  
打开酷Q目录下的 `conf\CQP.cfg` 文件，并在文件末尾插入以下两行，即可开启开发模式。  
- 您可以使用“Notepad++”、“记事本”等文本编辑器编辑本文件。  
- 如果本文件不存在，您可以先启动一次 酷Q，本文件将自动被生成。  
  
```  
[Dev]  
Enable=1  
```  
开启开发模式后，酷Q将新增以下功能，帮助您进行开发：  
- 错误信息将会详细显示。  
- 支持载入、打包未打包应用。（开启开发者模式后才能载入开发中的应用）  
- 在应用窗口，按住shift并单击“重启酷Q”，可以快速重载应用（而无需重启）。  
  
## 事件系统  
> 酷Q是**事件驱动**模式.  
  
大部分应用都是从收到一个事件开始.方式如下  
1. 在JSON中向酷Q申明要接收的事件  
2. 然后在C++中实现事件的接收函数  
  
### 1. JSON部分  
首先通过以下方式向酷Q申明要接收的事件  
```JSON  
{
	//其他部分与前面的示例一致  
	"event": //在这里增加  
	[  
		//新增的部分  
		{  
			"id"      : 1,                      // 事件ID  
			"type"    : 21,                     // 事件类型  
			"name"    : "私聊消息处理",          // 事件名称  
			"function": "functionPrivateMsg",   // 事件对应函数  
			"priority": 30000                   // 事件优先级(参见 cqp.im/deveventpriority)  
		}  
		//新增结束  
	]  
}
```  
|字段名称|说明|  
|:-|:-|  
|id|事件ID,保证唯一即可|  
|type|事件类型, `CQEVE.h`中有详细说明|  
|name|显示给用户的名称|  
|function|C++中处理事件的函数名字|  
|priority|优先级事件优先级(参见 [cqp.im/deveventpriority](http://cqp.im/deveventpriority "cqp.im/deveventpriority"))|  
|regex|(可选)只接受符合此项正则表达式的消息|  
  
`priority`**优先级**，整数，数值越大，优先级越低。  
目前支持以下四种优先级：

|数值|优先级|说明|
|:-|:-|:-|
|10000|最高|监控类应用，**无法**用于拦截消息。（如：消息数目统计等）|
|20000|高|消息控制类应用，可用于拦截消息。（如：机器人开关等）|
|30000|一般|普通功能类应用。（如：天气查询、游戏类等）|
|40000|低|聊天对话类应用。（如：词库、云词库）|
  
### 2. CPP部分  
然后在CPP文件中加入对应的处理函数  
```cpp  
#include "CQSDK/CQEVE_ALL.h"//这个头文件包含了所有事件的实现宏  
EVE_PrivateMsg_EX(functionPrivateMsg) {  
	//要执行的代码  
}  
```  
部分事件拥有有EX版本的实现宏,区别在于它以`_EX`结尾  
当事件拥有EX版宏时,强烈建议使用EX版进行实现  
在EX版本的事件函数中,可以用eve对象完成所有和事件有关的操作  
例如:  
```cpp  
EVE_PrivateMsg_EX(functionPrivateMsg) {  
	if (eve.isSystem())return;//不对系统消息做处理  
	int i = eve.font.气泡;  //获取对方的气泡类型  
  
	eve.sendMsg() << "你发送了这样的消息:" << endl << eve.message <<send;  
	eve.message_block();//拦截消息  
	//EX函数无需返回值,默认不拦截消息,如需拦截,请使用 eve.message_block();  
}  
```  
所有事件在`CQEVE.h`有详细的说明  
  
## CQAPI  
只是被动接受事件当然是不够的  
酷Q提供了一系列公开函数用于插件主动向酷Q提出各式各样的请求  
包括且不限于`日志写入`, `消息发送`, `信息获取`, `信息修改`等  
他们被实现于`CQAPI.h`和`CQAPI_EX.h`  
其中`CQAPI.h`是直接从酷Q导出的函数  
SDK增强并包装了它们,实现在`CQAPI_EX.h`  
请自行查阅  
  
注意,所有CQAPI的调用都需要权限,并且需要在json里申明  
```JSON  
{
	//其他部分与前面的示例一致  

	//修改部分
	"auth": [101,103,106]  
	//修改结束  
}
```  

|权限代码|说明|函数|
|:-|:-|:-|
|**20**|**[敏感]取Cookies**|**getCookies / getCsrfToken**|
|30|接收语音|getRecord|
|101|发送群消息|sendGroupMsg|
|103|发送讨论组消息|sendDiscussMsg|
|106|发送私聊消息|sendPrivateMsg|
|110|发送赞|sendLike|
|120|置群员移除|setGroupKick|
|121|置群员禁言|setGroupBan|
|122|置群管理员|setGroupAdmin|
|123|置全群禁言|setGroupWholeBan|
|124|置匿名群员禁言|setGroupAnonymousBan|
|125|置群匿名设置|setGroupAnonymous|
|126|置群成员名片|setGroupCard|
|**127**|**[敏感]置群退出**|**setGroupLeave**|
|128|置群成员专属头衔|setGroupSpecialTitle|
|130|取群成员信息|getGroupMemberInfoV2 / getGroupMemberInfo|
|131|取陌生人信息|getStrangerInfo|
|140|置讨论组退出|setDiscussLeave|
|150|置好友添加请求|setFriendAddRequest|
|151|置群添加请求|setGroupAddRequest|
|160|取群成员列表|getGroupMemberList|
|161|取群列表|getGroupList|
|180|撤回消息|deleteMsg|
|~~170~~|~~重启酷q~~|~~未知~~|

标注为**粗体**的Api涉及帐号权限等安全问题，因此在使用时会向用户进行“敏感权限”确认。  
  
## 其他高级选项  
### CQ码如何发送  
在`CQMsgCode.h`中的`code`类包含各种作者已知的CQ码工厂方法  
### 正则消息如何接受  
json部分略过..  
此条目待编辑  
### 消息和原始的不一样怎么办  
因为为了不把普通消息错当成CQ码,所以消息会被编码  
可以通过 `CQTools.h`的  
`std::string&msg_decode(std::string & s, bool isCQ = false);`函数将消息解码  
### 解码之后还是难以理解怎么办.
那就使用CodeMsgs工具

## CodeMsgs工具
这是一个用于解码消息的工具,  
不同于`std::string&msg_decode(std::string & s, bool isCQ = false);`函数  
这是一个工具集合,专门用于处理消息. 
使用方法如下:  
```cpp
CodeMsgs m("123456[CQ:sign,title=aaa:bbb,image=ccc=ddd]654321");
//首先初始化
//这时候消息将被解析成3段
//m[0].get()=="123456";
//m[1].get()=="sign";//对于CQ码来说,get是返回CQ类型
//m[1].get("title")=="aaa:bbb";//对于CQ码来说,get("")是返回对应的值
//m[2].get()=="654321";
//----------------------
//CodeMsgs 使用了数据库的操作思想.
//m[0]实际上是将指针定位到了第1段.
//也就是说
m[0];
bool True=m.get()=="123456";
m[1];
bool False=m.get()=="123456";
bool True=m.get()=="sign";//对于CQ码来说,get是返回CQ类型
//理解起来应该不困难
//随时使用以下函数判断是否是CQ码
bool True=m.isCode();
bool True=m.is("sign");
//当遇到未知CQ码时,还能遍历参数
auto keys = m.keys();
for each (auto key in keys)
	m.get(key);
//还能查找指定CQ码
m[0].find("sign");//从头找起
int pos=m.pos();//获得指针位置
//其实CodeMsgs本身也能迭代
for each (auto&code in m)
{
	if (code.isCQcode()) {
		auto keys = m.keys();

		cout << "{code:" << code.get() << ", args:{";
		for each (auto key in keys)
		{
			cout << "'" << key << "': '" << code.get(key) << "'" << ", ";
		}
		cout << "}";
	}
	else {
		cout << "[" << code.get() << "]";
	}
}
```

## 待续未完