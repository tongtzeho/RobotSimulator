#pragma once

#include "RGBSensor.h"

typedef struct _PyRGBSensor
{
private:
	PyObject_HEAD
public:
	RGBSensor *rgbSensor;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyRGBSensor *self, PyObject *args);
	static void PyDel(_PyRGBSensor *self);
	static PyObject* PyStr(_PyRGBSensor *self);
	static PyObject* PyRepr(_PyRGBSensor *self);
	static PyObject* PyIsEnabled(_PyRGBSensor *self);
	static PyObject* PySetEnabled(_PyRGBSensor *self, PyObject *args);
	static PyObject* PySaveBMP(_PyRGBSensor *self, PyObject *args);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyRGBSensor;