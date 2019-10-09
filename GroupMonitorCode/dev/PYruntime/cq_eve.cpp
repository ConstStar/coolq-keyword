#include "py_环境.h"
#include "py_变量.h"
#include "PyTupleGet.h"

#include <CQLogger.h>
#include <CQEVE_ALL.h>

using namespace std;
using namespace CQ;

static Logger logger("PyEve");

//if (!PY_BOOL) return 0;\
//##__VA_ARGS__
#define EVE_PY(PY_EVE, PY_BOOL, ...)\
if (!PY_EVE) return 0;\
auto *ret = PyEval_CallObject(PY_EVE, PyTupleGet(##__VA_ARGS__));\
if (PyErr_Occurred()) { logger.InfoSend("<" #PY_EVE ">事件未能正确执行"); PyErr_Clear(); return 0; }\
int result;	PyArg_Parse(ret, "i", &result);	logger.Debug() << "返回值:" << result << send;\
return result;\

EVE_Enable(Enable) {
	logger.Debug("Enable");
	static bool init = false;
	if (init) {
		logger.Debug("已加载");
	}
	else if (py_初始化()) {
		py_反射函数();

		logger.Info("加载成功");//初始化环境
	}
	else
		logger.Info("加载失败");

	return 0;
}

EVE_Exit(Exit) { EVE_PY(Exit_py, Exit_t) }
EVE_Disable(Disable) { EVE_PY(Disable_py, Disable_t) }
EVE_PrivateMsg(PrivateMsg1) { EVE_PY(PrivateMsg1_py, PrivateMsg1_t, subType, msgId, fromQQ, msg, font) }
EVE_PrivateMsg(PrivateMsg2) { EVE_PY(PrivateMsg2_py, PrivateMsg2_t, subType, msgId, fromQQ, msg, font) }
EVE_PrivateMsg(PrivateMsg3) { EVE_PY(PrivateMsg3_py, PrivateMsg3_t, subType, msgId, fromQQ, msg, font) }
EVE_PrivateMsg(PrivateMsg4) { EVE_PY(PrivateMsg4_py, PrivateMsg4_t, subType, msgId, fromQQ, msg, font) }
EVE_GroupMsg(GroupMsg1) { EVE_PY(GroupMsg1_py, GroupMsg1_t, subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font) }
EVE_GroupMsg(GroupMsg2) { EVE_PY(GroupMsg2_py, GroupMsg2_t, subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font) }
EVE_GroupMsg(GroupMsg3) { EVE_PY(GroupMsg3_py, GroupMsg3_t, subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font) }
EVE_GroupMsg(GroupMsg4) { EVE_PY(GroupMsg4_py, GroupMsg4_t, subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font) }
EVE_DiscussMsg(DiscussMsg1) { EVE_PY(DiscussMsg1_py, DiscussMsg1_t, subType, msgId, fromDiscuss, fromQQ, msg, font) }
EVE_DiscussMsg(DiscussMsg2) { EVE_PY(DiscussMsg2_py, DiscussMsg2_t, subType, msgId, fromDiscuss, fromQQ, msg, font) }
EVE_DiscussMsg(DiscussMsg3) { EVE_PY(DiscussMsg3_py, DiscussMsg3_t, subType, msgId, fromDiscuss, fromQQ, msg, font) }
EVE_DiscussMsg(DiscussMsg4) { EVE_PY(DiscussMsg4_py, DiscussMsg4_t, subType, msgId, fromDiscuss, fromQQ, msg, font) }
EVE_Friend_Add(Friend_Add1) { EVE_PY(Friend_Add1_py, Friend_Add1_t, subType, msgId, fromQQ) }
EVE_Friend_Add(Friend_Add2) { EVE_PY(Friend_Add2_py, Friend_Add2_t, subType, msgId, fromQQ) }
EVE_Friend_Add(Friend_Add3) { EVE_PY(Friend_Add3_py, Friend_Add3_t, subType, msgId, fromQQ) }
EVE_Friend_Add(Friend_Add4) { EVE_PY(Friend_Add4_py, Friend_Add4_t, subType, msgId, fromQQ) }
EVE_GroupUpload(GroupUpload1) { EVE_PY(GroupUpload1_py, GroupUpload1_t, subType, sendTime, fromGroup, fromQQ, file) }
EVE_GroupUpload(GroupUpload2) { EVE_PY(GroupUpload2_py, GroupUpload2_t, subType, sendTime, fromGroup, fromQQ, file) }
EVE_GroupUpload(GroupUpload3) { EVE_PY(GroupUpload3_py, GroupUpload3_t, subType, sendTime, fromGroup, fromQQ, file) }
EVE_GroupUpload(GroupUpload4) { EVE_PY(GroupUpload4_py, GroupUpload4_t, subType, sendTime, fromGroup, fromQQ, file) }
EVE_Request_AddFriend(Request_AddFriend1) { EVE_PY(Request_AddFriend1_py, Request_AddFriend1_t, subType, msgId, fromQQ, msg, responseFlag) }
EVE_Request_AddFriend(Request_AddFriend2) { EVE_PY(Request_AddFriend2_py, Request_AddFriend2_t, subType, msgId, fromQQ, msg, responseFlag) }
EVE_Request_AddFriend(Request_AddFriend3) { EVE_PY(Request_AddFriend3_py, Request_AddFriend3_t, subType, msgId, fromQQ, msg, responseFlag) }
EVE_Request_AddFriend(Request_AddFriend4) { EVE_PY(Request_AddFriend4_py, Request_AddFriend4_t, subType, msgId, fromQQ, msg, responseFlag) }
EVE_Request_AddGroup(Request_AddGroup1) { EVE_PY(Request_AddGroup1_py, Request_AddGroup1_t, subType, sendTime, fromGroup, fromQQ, msg, responseFlag) }
EVE_Request_AddGroup(Request_AddGroup2) { EVE_PY(Request_AddGroup2_py, Request_AddGroup2_t, subType, sendTime, fromGroup, fromQQ, msg, responseFlag) }
EVE_Request_AddGroup(Request_AddGroup3) { EVE_PY(Request_AddGroup3_py, Request_AddGroup3_t, subType, sendTime, fromGroup, fromQQ, msg, responseFlag) }
EVE_Request_AddGroup(Request_AddGroup4) { EVE_PY(Request_AddGroup4_py, Request_AddGroup4_t, subType, sendTime, fromGroup, fromQQ, msg, responseFlag) }
EVE_System_GroupAdmin(System_GroupAdmin1) { EVE_PY(System_GroupAdmin1_py, System_GroupAdmin1_t, subType, msgId, fromGroup, beingOperateQQ) }
EVE_System_GroupAdmin(System_GroupAdmin2) { EVE_PY(System_GroupAdmin2_py, System_GroupAdmin2_t, subType, msgId, fromGroup, beingOperateQQ) }
EVE_System_GroupAdmin(System_GroupAdmin3) { EVE_PY(System_GroupAdmin3_py, System_GroupAdmin3_t, subType, msgId, fromGroup, beingOperateQQ) }
EVE_System_GroupAdmin(System_GroupAdmin4) { EVE_PY(System_GroupAdmin4_py, System_GroupAdmin4_t, subType, msgId, fromGroup, beingOperateQQ) }
EVE_System_GroupMemberDecrease(System_GroupMemberDecrease1) { EVE_PY(System_GroupMemberDecrease1_py, System_GroupMemberDecrease1_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberDecrease(System_GroupMemberDecrease2) { EVE_PY(System_GroupMemberDecrease2_py, System_GroupMemberDecrease2_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberDecrease(System_GroupMemberDecrease3) { EVE_PY(System_GroupMemberDecrease3_py, System_GroupMemberDecrease3_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberDecrease(System_GroupMemberDecrease4) { EVE_PY(System_GroupMemberDecrease4_py, System_GroupMemberDecrease4_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberIncrease(System_GroupMemberIncrease1) { EVE_PY(System_GroupMemberIncrease1_py, System_GroupMemberIncrease1_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberIncrease(System_GroupMemberIncrease2) { EVE_PY(System_GroupMemberIncrease2_py, System_GroupMemberIncrease2_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberIncrease(System_GroupMemberIncrease3) { EVE_PY(System_GroupMemberIncrease3_py, System_GroupMemberIncrease3_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }
EVE_System_GroupMemberIncrease(System_GroupMemberIncrease4) { EVE_PY(System_GroupMemberIncrease4_py, System_GroupMemberIncrease4_t, subType, msgId, fromGroup, fromQQ, beingOperateQQ) }