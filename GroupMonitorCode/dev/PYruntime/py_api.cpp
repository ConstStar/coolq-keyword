#include "py_环境.h"

#include <CQLogger.h>
#include <CQAPI_EX.h>
#include"PyTupleMinute.h"
#include"PyTupleGet.h"

using namespace CQ;
using namespace std;

typedef PyObject* PYO;
static Logger logger("PyRunAPi");

//self - 调用方
//args - 参数
#define SPO(name) static PYO name (PYO self, PYO args)

SPO(py_addLog) {
	PYO res = nullptr;
	Py_BEGIN_ALLOW_THREADS

	整数型 优先级;string 类型;string 内容;
	PyTupleMinute r(args, &优先级, &类型, &内容);
	if (r) {
		res = PyTupleGet(addLog(优先级, 类型.c_str(), 内容.c_str()));
	}
	else
		logger.Debug() << "解析失败" << send;

	//if (PyArg_ParseTuple(args, "iss", &优先级, &类型, &内容)) {
	//	auto 类型G = G(类型), 内容G = G(内容);
	//	r = addLog(优先级, *类型G, *内容G);
	//	res = Py_BuildValue("(i)", r);
	//}
	//else
	//	logger.Debug() << "解析失败" << send;

	Py_END_ALLOW_THREADS;

	return res;
}
SPO(py_deleteMsg) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getAppDirectory) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getCookies) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getCsrfToken) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getGroupList) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getGroupMemberInfo) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getLoginNick) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getLoginQQ) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getRecord) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_getStrangerInfo) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_sendDiscussMsg) {
	PYO res = nullptr; Py_BEGIN_ALLOW_THREADS;
	长整数型 讨论组号; 文本型 msg;
	if (!PyArg_ParseTuple(args, "ls", &讨论组号, &msg))
		return NULL;
	整数型 r = CQ::sendDiscussMsg(讨论组号, msg);
	res = Py_BuildValue("(i)", r);
	Py_END_ALLOW_THREADS; return res;
}
SPO(py_sendGroupMsg) {
	PYO res = nullptr; Py_BEGIN_ALLOW_THREADS;
	长整数型 群号; 文本型 msg;
	if (!PyArg_ParseTuple(args, "ls", &群号, &msg))
		return NULL;
	整数型 r = CQ::sendGroupMsg(群号, msg);
	res = Py_BuildValue("(i)", r);
	Py_END_ALLOW_THREADS; return res;
}
SPO(py_sendLike) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_sendPrivateMsg) {
	PYO res = nullptr; Py_BEGIN_ALLOW_THREADS;
	长整数型 QQ; 文本型 msg;
	if (!PyArg_ParseTuple(args, "ls", &QQ, &msg))
		return NULL;
	整数型 r = CQ::sendPrivateMsg(QQ, msg);
	res = Py_BuildValue("(i)", r);
	Py_END_ALLOW_THREADS; return res;
}
SPO(py_setDiscussLeave) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setFatal) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setFriendAddRequest) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupAddRequest) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupAdmin) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupAnonymous) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupAnonymousBan) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupBan) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupCard) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupKick) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupLeave) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupSpecialTitle) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }
SPO(py_setGroupWholeBan) { PYO res = nullptr; Py_BEGIN_ALLOW_THREADS; Py_END_ALLOW_THREADS; return res; }

static PyMethodDef API方法数组[] = {
	{ "addLog"              ,py_addLog, METH_VARARGS , ""},
	//{ "deleteMsg"           ,py_deleteMsg, METH_VARARGS , ""},
	//{ "getAppDirectory"     ,py_getAppDirectory, METH_VARARGS , ""},
	//{ "getCookies"          ,py_getCookies, METH_VARARGS , ""},
	//{ "getCsrfToken"        ,py_getCsrfToken, METH_VARARGS ,"" },
	//{ "getGroupList"        ,py_getGroupList, METH_VARARGS , ""},
	//{ "getGroupMemberInfo"  ,py_getGroupMemberInfo, METH_VARARGS ,"" },
	//{ "getLoginNick"        ,py_getLoginNick, METH_VARARGS , ""},
	//{ "getLoginQQ"          ,py_getLoginQQ, METH_VARARGS ,"" },
	//{ "getRecord"           ,py_getRecord, METH_VARARGS , ""},
	//{ "getStrangerInfo"     ,py_getStrangerInfo, METH_VARARGS ,"" },
	//{ "sendDiscussMsg"      ,py_sendDiscussMsg, METH_VARARGS , ""},
	//{ "sendGroupMsg"        ,py_sendGroupMsg, METH_VARARGS , ""},
	//{ "sendLike"            ,py_sendLike, METH_VARARGS ,"" },
	//{ "sendPrivateMsg"      ,py_sendPrivateMsg, METH_VARARGS , ""},
	//{ "setDiscussLeave"     ,py_setDiscussLeave, METH_VARARGS , ""},
	//{ "setFatal"            ,py_setFatal, METH_VARARGS ,"" },
	//{ "setFriendAddRequest" ,py_setFriendAddRequest, METH_VARARGS ,"" },
	//{ "setGroupAddRequest"  ,py_setGroupAddRequest, METH_VARARGS , ""},
	//{ "setGroupAdmin"       ,py_setGroupAdmin, METH_VARARGS ,"" },
	//{ "setGroupAnonymous"   ,py_setGroupAnonymous, METH_VARARGS ,"" },
	//{ "setGroupAnonymousBan",py_setGroupAnonymousBan, METH_VARARGS , ""},
	//{ "setGroupBan"         ,py_setGroupBan, METH_VARARGS ,"" },
	//{ "setGroupCard"        ,py_setGroupCard, METH_VARARGS , ""},
	//{ "setGroupKick"        ,py_setGroupKick, METH_VARARGS , ""},
	//{ "setGroupLeave"       ,py_setGroupLeave, METH_VARARGS ,"" },
	//{ "setGroupSpecialTitle",py_setGroupSpecialTitle, METH_VARARGS , ""},
	//{ "setGroupWholeBan"    ,py_setGroupWholeBan, METH_VARARGS ,"" },

	{ NULL                  , NULL,0,NULL },
};

static struct PyModuleDef API描述 = {
	PyModuleDef_HEAD_INIT,
	"CQ",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	API方法数组
};
PYO getCQAPI() {
	return PyModule_Create(&API描述);
}