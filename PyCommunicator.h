#pragma once

#include "Communicator.h"

typedef struct _PyCommunicator
{
private:
	PyObject_HEAD
public:
	Communicator *communicator;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyCommunicator *self, PyObject *args);
	static void PyDel(_PyCommunicator *self);
	static PyObject* PyStr(_PyCommunicator *self);
	static PyObject* PyRepr(_PyCommunicator *self);
	static PyObject* PySend(_PyCommunicator *self, PyObject *args);
	static PyObject* PyRead(_PyCommunicator *self);
	static PyObject* PyGetPosition(_PyCommunicator *self);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyCommunicator;
