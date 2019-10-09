#pragma once
#include <Python.h>
#include<string>
#include "py_зЊТы.h"

inline PyObject* toPyObj(int arg) { return Py_BuildValue("i", arg); }
inline bool setPyObj(PyObject*obj, int*arg) { return PyArg_ParseTuple(obj, "i", arg); }

inline PyObject* toPyObj(long long arg) { return Py_BuildValue("L", arg); }
inline bool setPyObj(PyObject*obj, long long*arg) { return PyArg_ParseTuple(obj, "L", arg); }

inline PyObject* toPyObj(const char* arg) { auto tepptr = U(arg); char*tepU = *tepptr; return Py_BuildValue("s", tepU); }
inline bool setPyObj(PyObject*obj, std::string*arg) {
	char*g;
	bool res = PyArg_ParseTuple(obj, "z", g);
	if (res) {
		auto tepptr = G(g);
		arg->clear();
		arg->append(g);
	}
	return res;
}