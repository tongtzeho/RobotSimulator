#pragma once

#include "IActionController.h"

typedef struct _PyActionController
{
private:
	PyObject_HEAD
public:
	IActionController *actionController;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyActionController *self, PyObject *args);
	static void PyDel(_PyActionController *self);
	static PyObject* PyStr(_PyActionController *self);
	static PyObject* PyRepr(_PyActionController *self);
	static PyObject* PyGetState(_PyActionController *self);
	static PyObject* PySetState(_PyActionController *self, PyObject *args);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyActionController;