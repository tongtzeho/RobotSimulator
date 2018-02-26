#include "../Engine/CoolEngine.h"

#include "PyRGBSensor.h"

using namespace CE;

extern "C"
{
	PyObject* _PyRGBSensor::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyRGBSensor *self = (PyRGBSensor*)type->tp_alloc(type, 0);
		self->rgbSensor = nullptr;
		return (PyObject*)self;
	}

	int _PyRGBSensor::PyInit(_PyRGBSensor *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<RGBSensor> *compRGBSensor = dynamic_cast<Component<RGBSensor>*>(pyComp->component);
		if (compRGBSensor == nullptr)
		{
			return -1;
		}
		self->rgbSensor = (RGBSensor*)**compRGBSensor;
		if (self->rgbSensor == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyRGBSensor::PyDel(_PyRGBSensor *self)
	{
		return;
	}

	PyObject* _PyRGBSensor::PyStr(_PyRGBSensor *self)
	{
		char str[64] = { 0 };
		sprintf(str, "rgbSensor@0x%llX", reinterpret_cast<long long>(self->rgbSensor));
		return PyString_FromString(str);
	}

	PyObject* _PyRGBSensor::PyRepr(_PyRGBSensor *self)
	{
		return PyStr(self);
	}

	PyObject* _PyRGBSensor::PyIsEnabled(_PyRGBSensor *self)
	{
		return PyBool_FromLong(self->rgbSensor->IsEnabled());
	}

	PyObject* _PyRGBSensor::PySetEnabled(_PyRGBSensor *self, PyObject *args)
	{
		int n;
		if (!PyArg_ParseTuple(args, "i", &n))
		{
			return NULL;
		}
		self->rgbSensor->SetEnabled(n != 0);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* _PyRGBSensor::PySaveBMP(_PyRGBSensor *self, PyObject *args)
	{
		const char *str;
		if (!PyArg_ParseTuple(args, "s", &str))
		{
			return NULL;
		}
		bool result = self->rgbSensor->SaveBMP(str);
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

PyMemberDef _PyRGBSensor::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyRGBSensor::PyMethodMembers[] = {
	{ "isEnabled", (PyCFunction)PyIsEnabled, METH_NOARGS },
	{ "setEnabled", (PyCFunction)PySetEnabled, METH_VARARGS },
	{ "saveBMP", (PyCFunction)PySaveBMP, METH_VARARGS },
	{ NULL, NULL }
};

PyTypeObject _PyRGBSensor::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.rgbSensor", /* tp_name */
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