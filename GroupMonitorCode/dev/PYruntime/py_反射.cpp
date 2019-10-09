#include "py_变量.h"
#include "py_环境.h"

void py_反射函数() {
	Exit_py = PyObject_GetAttrString(RunTime, "Exit");

	Enable_py = PyObject_GetAttrString(RunTime, "Enable");

	Disable_py = PyObject_GetAttrString(RunTime, "Disable");

	PrivateMsg1_py = PyObject_GetAttrString(RunTime, "PrivateMsg1");
	PrivateMsg2_py = PyObject_GetAttrString(RunTime, "PrivateMsg2");
	PrivateMsg3_py = PyObject_GetAttrString(RunTime, "PrivateMsg3");
	PrivateMsg4_py = PyObject_GetAttrString(RunTime, "PrivateMsg4");

	GroupMsg1_py = PyObject_GetAttrString(RunTime, "GroupMsg1");
	GroupMsg2_py = PyObject_GetAttrString(RunTime, "GroupMsg2");
	GroupMsg3_py = PyObject_GetAttrString(RunTime, "GroupMsg3");
	GroupMsg4_py = PyObject_GetAttrString(RunTime, "GroupMsg4");

	DiscussMsg1_py = PyObject_GetAttrString(RunTime, "DiscussMsg1");
	DiscussMsg2_py = PyObject_GetAttrString(RunTime, "DiscussMsg2");
	DiscussMsg3_py = PyObject_GetAttrString(RunTime, "DiscussMsg3");
	DiscussMsg4_py = PyObject_GetAttrString(RunTime, "DiscussMsg4");

	Friend_Add1_py = PyObject_GetAttrString(RunTime, "Friend_Add1");
	Friend_Add2_py = PyObject_GetAttrString(RunTime, "Friend_Add2");
	Friend_Add3_py = PyObject_GetAttrString(RunTime, "Friend_Add3");
	Friend_Add4_py = PyObject_GetAttrString(RunTime, "Friend_Add4");

	GroupUpload1_py = PyObject_GetAttrString(RunTime, "GroupUpload1");
	GroupUpload2_py = PyObject_GetAttrString(RunTime, "GroupUpload2");
	GroupUpload3_py = PyObject_GetAttrString(RunTime, "GroupUpload3");
	GroupUpload4_py = PyObject_GetAttrString(RunTime, "GroupUpload4");

	Request_AddFriend1_py = PyObject_GetAttrString(RunTime, "Request_AddFriend1");
	Request_AddFriend2_py = PyObject_GetAttrString(RunTime, "Request_AddFriend2");
	Request_AddFriend3_py = PyObject_GetAttrString(RunTime, "Request_AddFriend3");
	Request_AddFriend4_py = PyObject_GetAttrString(RunTime, "Request_AddFriend4");

	Request_AddGroup1_py = PyObject_GetAttrString(RunTime, "Request_AddGroup1");
	Request_AddGroup2_py = PyObject_GetAttrString(RunTime, "Request_AddGroup2");
	Request_AddGroup3_py = PyObject_GetAttrString(RunTime, "Request_AddGroup3");
	Request_AddGroup4_py = PyObject_GetAttrString(RunTime, "Request_AddGroup4");

	System_GroupAdmin1_py = PyObject_GetAttrString(RunTime, "System_GroupAdmin1");
	System_GroupAdmin2_py = PyObject_GetAttrString(RunTime, "System_GroupAdmin2");
	System_GroupAdmin3_py = PyObject_GetAttrString(RunTime, "System_GroupAdmin3");
	System_GroupAdmin4_py = PyObject_GetAttrString(RunTime, "System_GroupAdmin4");

	System_GroupMemberDecrease1_py = PyObject_GetAttrString(RunTime, "System_GroupMemberDecrease1");
	System_GroupMemberDecrease2_py = PyObject_GetAttrString(RunTime, "System_GroupMemberDecrease2");
	System_GroupMemberDecrease3_py = PyObject_GetAttrString(RunTime, "System_GroupMemberDecrease3");
	System_GroupMemberDecrease4_py = PyObject_GetAttrString(RunTime, "System_GroupMemberDecrease4");

	System_GroupMemberIncrease1_py = PyObject_GetAttrString(RunTime, "System_GroupMemberIncrease1");
	System_GroupMemberIncrease2_py = PyObject_GetAttrString(RunTime, "System_GroupMemberIncrease2");
	System_GroupMemberIncrease3_py = PyObject_GetAttrString(RunTime, "System_GroupMemberIncrease3");
	System_GroupMemberIncrease4_py = PyObject_GetAttrString(RunTime, "System_GroupMemberIncrease4");

	PyErr_Clear();
}