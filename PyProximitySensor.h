#pragma once

#include "ProximitySensor.h"

typedef struct _PyProximitySensor
{
private:
	PyObject_HEAD
public:
	ProximitySensor *proximitySensor;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyProximitySensor *self, PyObject *args);
	static void PyDel(_PyProximitySensor *self);
	static PyObject* PyStr(_PyProximitySensor *self);
	static PyObject* PyRepr(_PyProximitySensor *self);
	static PyObject* PyGetData(_PyProximitySensor *self);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyProximitySensor;