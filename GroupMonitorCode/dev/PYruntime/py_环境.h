#pragma once

#include <Python.h>

#ifdef _DEBUG
#pragma comment(lib,"SDK_Debug.lib")
#else
#pragma comment(lib,"SDK_Release.lib")
#endif

#define CQAPPID "cn.mikupy2001.PYruntime"

extern PyObject *RunTime;


bool py_��ʼ��();
void py_���亯��();
PyObject *getCQAPI();