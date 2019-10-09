#define WIN32_LEAN_AND_MEAN
#include "py_环境.h"
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include <Python.h>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace CQ;
static Logger logger("PyInit");

PyObject *RunTime;

bool py_初始化() {
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	string bins(pBuf), locs(pBuf);

	bins += "\\bin\\python36.dll";
	locs += "\\python36.dll";

	if (!LoadLibrary("python36.dll"))
		if (!LoadLibrary(bins.c_str()))
			if (!LoadLibrary(locs.c_str())) {
				logger.Debug(pBuf);
				logger.Warning("无法找到并加载python36.dll,您下载的环境不完整!");
				return false;
			}

	Py_Initialize();
	if (!Py_IsInitialized())
	{
		logger.Warning("py环境初始化失败");
		return false;
	}

	PyRun_SimpleString("import sys;");
	PyRun_SimpleString("sys.path.append(\"app-python\");");

	RunTime = PyImport_ImportModule("PluginManager");
	if (!RunTime) {
		logger.Warning("py文件加载失败");
		Py_Finalize();
		return false;
	}
	auto cq = getCQAPI();
	logger.Debug() << "CQ模块地址:" << (int)cq << send;
	auto r = PyModule_AddObject(RunTime, "CQ", cq);
	logger.Debug() << "添加到RunTime结果:" << r << send;

	return true;
}