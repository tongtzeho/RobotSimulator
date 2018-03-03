#include "../Engine/CoolEngine.h"

#include "PyCameraSensor.h"

using namespace CE;

extern "C"
{
	PyObject* _PyCameraSensor::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyCameraSensor *self = (PyCameraSensor*)type->tp_alloc(type, 0);
		self->cameraSensor = nullptr;
		return (PyObject*)self;
	}

	int _PyCameraSensor::PyInit(_PyCameraSensor *self, PyObject *args)
	{
		return -1; // 通过sim.rgbSensor或sim.depthSensor初始化
	}

	void _PyCameraSensor::PyDel(_PyCameraSensor *self)
	{
		return;
	}

	PyObject* _PyCameraSensor::PyStr(_PyCameraSensor *self)
	{
		char str[64] = { 0 };
		sprintf(str, "&%s@0x%llX", self->cameraSensor->GetSensorType(), reinterpret_cast<long long>(self->cameraSensor));
		return PyString_FromString(str);
	}

	PyObject* _PyCameraSensor::PyRepr(_PyCameraSensor *self)
	{
		return PyStr(self);
	}

	PyObject* _PyCameraSensor::PyIsEnabled(_PyCameraSensor *self)
	{
		return PyBool_FromLong(self->cameraSensor->IsEnabled());
	}

	PyObject* _PyCameraSensor::PySetEnabled(_PyCameraSensor *self, PyObject *args)
	{
		int n;
		if (!PyArg_ParseTuple(args, "i", &n))
		{
			return NULL;
		}
		self->cameraSensor->SetEnabled(n != 0);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* _PyCameraSensor::PyGetData(_PyCameraSensor *self)
	{
		unsigned height;
		unsigned width;
		unsigned channel;
		const float *textureData = self->cameraSensor->GetTextureData(height, width, channel);
		PyObject *pyList;
		pyList = PyList_New(height);
		for (unsigned i = 0; i < height; ++i)
		{
			PyObject *rowData = PyList_New(width);
			for (unsigned j = 0; j < width; ++j)
			{
				if (channel == 1)
				{
					PyList_SetItem(rowData, j, PyFloat_FromDouble(Math::Clamp(textureData[i*width + j], 0.0f, 1.0f)));
				}
				else
				{
					PyObject *pixel = PyList_New(channel);
					for (unsigned k = 0; k < channel; ++k)
					{
						PyList_SetItem(pixel, k, PyFloat_FromDouble(Math::Clamp(textureData[(i*width + j)*channel + k], 0.0f, 1.0f)));
					}
					PyList_SetItem(rowData, j, pixel);
				}
			}
			PyList_SetItem(pyList, i, rowData);
		}
		return pyList;
	}

	PyObject* _PyCameraSensor::PySaveBMP(_PyCameraSensor *self, PyObject *args)
	{
		const char *str;
		if (!PyArg_ParseTuple(args, "s", &str))
		{
			return NULL;
		}
		bool result = self->cameraSensor->SaveBMP(str);
		if (result)
		{
			return PyString_FromString(str);
		}
		else
		{
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
}

PyMemberDef _PyCameraSensor::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyCameraSensor::PyMethodMembers[] = {
	{ "isEnabled", (PyCFunction)PyIsEnabled, METH_NOARGS },
	{ "setEnabled", (PyCFunction)PySetEnabled, METH_VARARGS },
	{ "getData", (PyCFunction)PyGetData, METH_NOARGS },
	{ "saveBMP", (PyCFunction)PySaveBMP, METH_VARARGS },
	{ NULL, NULL }
};

PyTypeObject _PyCameraSensor::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.cameraSensor", /* tp_name */
	sizeof(PyComponent), /* tp_basicsize */
	0, /* tp_itemsize */
	(destructor)PyDel, /* tp_dealloc */
	0, /* tp_print */
	0, /* tp_getattr */
	0, /* tp_setattr */
	0, /* tp_compare */
	(reprfunc)PyRepr, /* tp_repr */
	0, /* tp_as_number */
	0, /* tp_as_sequence */
	0, /* tp_as_mapping */
	0, /* tp_hash */
	0, /* tp_call */
	(reprfunc)PyStr, /* tp_str */
	0, /* tp_getattro */
	0, /* tp_setattro */
	0, /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	NULL, /* tp_doc */
	0, /* tp_traverse */
	0, /* tp_clear */
	0, /* tp_richcompare */
	0, /* tp_weaklistoffset */
	0, /* tp_iter */
	0, /* tp_iternext */
	PyMethodMembers, /* tp_methods */
	PyDataMembers, /* tp_members */
	0, /* tp_getset */
	0, /* tp_base */
	0, /* tp_dict */
	0, /* tp_descr_get */
	0, /* tp_descr_set */
	0, /* tp_dictoffset */
	(initproc)PyInit, /* tp_init */
	0, /* tp_alloc */
	PyNew /* tp_new */
};