#include "PyIMUSensor.h"

using namespace CE;

extern "C"
{
	PyObject* _PyIMUSensor::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyIMUSensor *self = (PyIMUSensor*)type->tp_alloc(type, 0);
		self->imuSensor = nullptr;
		return (PyObject*)self;
	}

	int _PyIMUSensor::PyInit(_PyIMUSensor *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<IMUSensor> *compIMUSensor = dynamic_cast<Component<IMUSensor>*>(pyComp->component);
		if (compIMUSensor == nullptr)
		{
			return -1;
		}
		self->imuSensor = (IMUSensor*)**compIMUSensor;
		if (self->imuSensor == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyIMUSensor::PyDel(_PyIMUSensor *self)
	{
		return;
	}

	PyObject* _PyIMUSensor::PyStr(_PyIMUSensor *self)
	{
		char str[64] = { 0 };
		sprintf(str, "imuSensor{'%s'}@0x%llX", self->imuSensor->GetComponent()->GetEntity()->GetName(), reinterpret_cast<long long>(self->imuSensor));
		return PyString_FromString(str);
	}

	PyObject* _PyIMUSensor::PyRepr(_PyIMUSensor *self)
	{
		return PyStr(self);
	}

	PyObject* _PyIMUSensor::PyGetData(_PyIMUSensor *self)
	{
		const Vector3 &acceleration = self->imuSensor->GetAcceleration();
		PyVector3 *result = (PyVector3*)_PyObject_New(const_cast<PyTypeObject*>(PyVector3::GetPyClassInfo()));
		result->x = acceleration.x;
		result->y = acceleration.y;
		result->z = acceleration.z;
		PyObject *ret = Py_BuildValue("O", result);
		Py_XDECREF(result);
		return ret;
	}
}

PyMemberDef _PyIMUSensor::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyIMUSensor::PyMethodMembers[] = {
	{ "getData", (PyCFunction)PyGetData, METH_NOARGS },
	{ NULL, NULL }
};

PyTypeObject _PyIMUSensor::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.imuSensor", /* tp_name */
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