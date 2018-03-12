#include "PyProximitySensor.h"

using namespace CE;

extern "C"
{
	PyObject* _PyProximitySensor::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyProximitySensor *self = (PyProximitySensor*)type->tp_alloc(type, 0);
		self->proximitySensor = nullptr;
		return (PyObject*)self;
	}

	int _PyProximitySensor::PyInit(_PyProximitySensor *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<ProximitySensor> *compProximitySensor = dynamic_cast<Component<ProximitySensor>*>(pyComp->component);
		if (compProximitySensor == nullptr)
		{
			return -1;
		}
		self->proximitySensor = (ProximitySensor*)**compProximitySensor;
		if (self->proximitySensor == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyProximitySensor::PyDel(_PyProximitySensor *self)
	{
		return;
	}

	PyObject* _PyProximitySensor::PyStr(_PyProximitySensor *self)
	{
		char str[64] = { 0 };
		sprintf(str, "proximitySensor{'%s'}@0x%llX", self->proximitySensor->GetComponent()->GetEntity()->GetName(), reinterpret_cast<long long>(self->proximitySensor));
		return PyString_FromString(str);
	}

	PyObject* _PyProximitySensor::PyRepr(_PyProximitySensor *self)
	{
		return PyStr(self);
	}

	PyObject* _PyProximitySensor::PyGetData(_PyProximitySensor *self)
	{
		return PyBool_FromLong(self->proximitySensor->IsNear());
	}
}

PyMemberDef _PyProximitySensor::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyProximitySensor::PyMethodMembers[] = {
	{ "getData", (PyCFunction)PyGetData, METH_NOARGS },
	{ NULL, NULL }
};

PyTypeObject _PyProximitySensor::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.proximitySensor", /* tp_name */
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