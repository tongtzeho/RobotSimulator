#pragma once

#include "IMUSensor.h"

typedef struct _PyIMUSensor
{
private:
	PyObject_HEAD
public:
	IMUSensor *imuSensor;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyIMUSensor *self, PyObject *args);
	static void PyDel(_PyIMUSensor *self);
	static PyObject* PyStr(_PyIMUSensor *self);
	static PyObject* PyRepr(_PyIMUSensor *self);
	static PyObject* PyGetData(_PyIMUSensor *self);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyIMUSensor;