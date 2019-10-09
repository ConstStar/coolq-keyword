#ifdef _DEBUG
#pragma comment(lib,"SDK_Debug.lib")
#else
#pragma comment(lib,"SDK_Release.lib")
#endif

//----我为什么要修改这个ID----
//这是您的插件名称,一个独一无二的名称

//----我应该如何修改这个ID----
//首先您应该修改下方的CQAPPID
//然后修改项目名称      (它会指引VS使用这个名称生成文件)
//最后修改json文件名称

//----我应该修改成什么----
//请查阅 http://d.cqp.me/Pro/开发/基础信息
#define CQAPPID "cn.xiaoxiaoge.GroupMonitor"


#define CQAPPINFO CQAPIVERTEXT "," CQAPPID

//接下来请查阅 example 以快速学习本SDK
