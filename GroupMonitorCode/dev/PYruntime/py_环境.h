#pragma once

#include <Python.h>

#ifdef _DEBUG
#pragma comment(lib,"SDK_Debug.lib")
#else
#pragma comment(lib,"SDK_Release.lib")
#endif

#define CQAPPID "cn.mikupy2001.PYruntime"

extern PyObject *RunTime;


bool py_初始化();
void py_反射函数();
PyObject *getCQAPI();