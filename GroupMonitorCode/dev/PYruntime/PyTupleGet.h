#pragma once
#include <Python.h>
#include <vector>

#include "Py2C.h"

//ÓÃ·¨
//PyObject* newPyObj=PyTupleGet(int,long long,const char*);
class PyTupleGet {
	template<class T, class...Args>
	void add(T one, Args...args);
	void add();
public:
	std::vector<PyObject*> objs;
	template<class...Args>
	PyTupleGet(Args...args);

	PyObject* getObj();
	operator PyObject*();
};

template<class ...Args>
inline PyTupleGet::PyTupleGet(Args ...args) { add(args...); }

template<class T, class ...Args>
inline void PyTupleGet::add(T one, Args ...args) { objs.push_back(toPyObj(one)); add(args...); }

inline void PyTupleGet::add() {}

inline PyObject * PyTupleGet::getObj() {
	auto obj = PyTuple_New(objs.size());
	for (unsigned int i = 0; i < objs.size(); i++)
		PyTuple_SetItem(obj, i, objs[i]);
	return obj;
}

inline PyTupleGet::operator PyObject*() { return getObj(); }