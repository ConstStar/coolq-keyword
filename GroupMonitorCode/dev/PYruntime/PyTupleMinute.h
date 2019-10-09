#pragma once
#include <Python.h>
//#include <utility>

#include "Py2C.h"

class PyTupleMinute {
	PyObject* Tuple;
	int index = 0;
	bool ok = false;

	template<class T, class...Args>
	void set(T one, Args...args);
	void set() { ok = true; };

public:
	template<class...Args>
	PyTupleMinute(PyObject* Tuple, Args...args);

	operator bool() { return ok; };
};

template<class ...Args>
inline PyTupleMinute::PyTupleMinute(PyObject * Tuple, Args ... args) { this->Tuple = Tuple; set(args...); }

template<class T, class ...Args>
inline void PyTupleMinute::set(T one, Args ... args)
{
	PyObject* obj = PyTuple_GetItem(Tuple, index++);
	if (setPyObj(obj, one))
		set(args...);
}
