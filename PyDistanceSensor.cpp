#include "PyDistanceSensor.h"

using namespace CE;

extern "C"
{
	PyObject* _PyDistanceSensor::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyDistanceSensor *self = (PyDistanceSensor*)type->tp_alloc(type, 0);
		self->distanceSensor = nullptr;
		return (PyObject*)self;
	}

	int _PyDistanceSensor::PyInit(_PyDistanceSensor *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<DistanceSensor> *compDistanceSensor = dynamic_cast<Component<DistanceSensor>*>(pyComp->component);
		if (compDistanceSensor == nullptr)
		{
			return -1;
		}
		self->distanceSensor = (DistanceSensor*)**compDistanceSensor;
		if (self->distanceSensor == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyDistanceSensor::PyDel(_PyDistanceSensor *self)
	{
		return;
	}

	PyObject* _PyDistanceSensor::PyStr(_PyDistanceSensor *self)
	{
		char str[64] = { 0 };
		sprintf(str, "distanceSensor{'%s'}@0x%llX", self->distanceSensor->GetComponent()->GetEntity()->GetName(), reinterpret_cast<long long>(self->distanceSensor));
		return PyString_FromString(str);
	}

	PyObject* _PyDistanceSensor::PyRepr(_PyDistanceSensor *self)
	{
		return PyStr(self);
	}

	PyObject* _PyDistanceSensor::PyGetData(_PyDistanceSensor *self)
	{
		float distance;
		if (self->distanceSensor->GetDistance(distance))
		{
			return PyFloat_FromDouble(distance);
		}
		else
		{
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
}

PyMemberDef _PyDistanceSensor::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyDistanceSensor::PyMethodMembers[] = {
	{ "getData", (PyCFunction)PyGetData, METH_NOARGS },
	{ NULL, NULL }
};

PyTypeObject _PyDistanceSensor::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.distanceSensor", /* tp_name */
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