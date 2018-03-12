#pragma once

#include "DistanceSensor.h"

typedef struct _PyDistanceSensor
{
private:
	PyObject_HEAD
public:
	DistanceSensor *distanceSensor;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyDistanceSensor *self, PyObject *args);
	static void PyDel(_PyDistanceSensor *self);
	static PyObject* PyStr(_PyDistanceSensor *self);
	static PyObject* PyRepr(_PyDistanceSensor *self);
	static PyObject* PyGetData(_PyDistanceSensor *self);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyDistanceSensor;