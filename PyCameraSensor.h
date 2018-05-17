#pragma once

#include "ICameraSensor.h"

typedef struct _PyCameraSensor // RGBSensor∫ÕDepthSensorπ≤”√_PyCameraSensor
{
private:
	PyObject_HEAD
public:
	ICameraSensor *cameraSensor;
	static PyObject* PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds);
	static int PyInit(_PyCameraSensor *self, PyObject *args);
	static void PyDel(_PyCameraSensor *self);
	static PyObject* PyStr(_PyCameraSensor *self);
	static PyObject* PyRepr(_PyCameraSensor *self);
	static PyObject* PyIsEnabled(_PyCameraSensor *self);
	static PyObject* PySetEnabled(_PyCameraSensor *self, PyObject *args);
	static PyObject* PyGetData(_PyCameraSensor *self);
	static PyObject* PyGetRemoteData(_PyCameraSensor *self, PyObject *args);
	static PyObject* PySaveBMP(_PyCameraSensor *self, PyObject *args);
private:
	static PyMemberDef PyDataMembers[];
	static PyMethodDef PyMethodMembers[];
	static PyTypeObject PyClassInfo;
public:
	inline static const PyTypeObject *GetPyClassInfo() { return &PyClassInfo; }
}PyCameraSensor;