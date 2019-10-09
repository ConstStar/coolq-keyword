#define WIN32_LEAN_AND_MEAN
#include "py_����.h"
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include <Python.h>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace CQ;
static Logger logger("PyInit");

PyObject *RunTime;

bool py_��ʼ��() {
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	string bins(pBuf), locs(pBuf);

	bins += "\\bin\\python36.dll";
	locs += "\\python36.dll";

	if (!LoadLibrary("python36.dll"))
		if (!LoadLibrary(bins.c_str()))
			if (!LoadLibrary(locs.c_str())) {
				logger.Debug(pBuf);
				logger.Warning("�޷��ҵ�������python36.dll,�����صĻ���������!");
				return false;
			}

	Py_Initialize();
	if (!Py_IsInitialized())
	{
		logger.Warning("py������ʼ��ʧ��");
		return false;
	}

	PyRun_SimpleString("import sys;");
	PyRun_SimpleString("sys.path.append(\"app-python\");");

	RunTime = PyImport_ImportModule("PluginManager");
	if (!RunTime) {
		logger.Warning("py�ļ�����ʧ��");
		Py_Finalize();
		return false;
	}
	auto cq = getCQAPI();
	logger.Debug() << "CQģ���ַ:" << (int)cq << send;
	auto r = PyModule_AddObject(RunTime, "CQ", cq);
	logger.Debug() << "��ӵ�RunTime���:" << r << send;

	return true;
}