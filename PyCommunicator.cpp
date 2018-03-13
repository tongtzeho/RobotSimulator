#include "PyCommunicator.h"

using namespace CE;

extern "C"
{
	PyObject* _PyCommunicator::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyCommunicator *self = (PyCommunicator*)type->tp_alloc(type, 0);
		self->communicator = nullptr;
		return (PyObject*)self;
	}

	int _PyCommunicator::PyInit(_PyCommunicator *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<Communicator> *compCommunicator = dynamic_cast<Component<Communicator>*>(pyComp->component);
		if (compCommunicator == nullptr)
		{
			return -1;
		}
		self->communicator = (Communicator*)**compCommunicator;
		if (self->communicator == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyCommunicator::PyDel(_PyCommunicator *self)
	{
		return;
	}

	PyObject* _PyCommunicator::PyStr(_PyCommunicator *self)
	{
		char str[64] = { 0 };
		sprintf(str, "communicator{'%s'}@0x%llX", self->communicator->GetComponent()->GetEntity()->GetName(), reinterpret_cast<long long>(self->communicator));
		return PyString_FromString(str);
	}

	PyObject* _PyCommunicator::PyRepr(_PyCommunicator *self)
	{
		return PyStr(self);
	}

	PyObject* _PyCommunicator::PySend(_PyCommunicator *self, PyObject *args)
	{
		const char *msg;
		if (!PyArg_ParseTuple(args, "s", &msg))
		{
			return NULL;
		}
		self->communicator->Send(msg);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* _PyCommunicator::PyRead(_PyCommunicator *self)
	{
		std::vector<std::string> messages;
		self->communicator->Read(messages);
		PyObject *pyList;
		pyList = PyList_New(messages.size());
		for (size_t i = 0; i < messages.size(); ++i)
		{
			PyList_SetItem(pyList, i, PyString_FromString(messages[i].c_str()));
		}
		return pyList;
	}

	PyObject* _PyCommunicator::PyGetPosition(_PyCommunicator *self)
	{
		PyVector3 *result = (PyVector3*)_PyObject_New(const_cast<PyTypeObject*>(PyVector3::GetPyClassInfo()));
		const Vector3 &position = self->communicator->GetPosition();
		result->x = position.x;
		result->y = position.y;
		result->z = position.z;
		PyObject *ret = Py_BuildValue("O", result);
		Py_XDECREF(result);
		return ret;
	}
}

PyMemberDef _PyCommunicator::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyCommunicator::PyMethodMembers[] = {
	{ "send", (PyCFunction)PySend, METH_VARARGS },
	{ "read", (PyCFunction)PyRead, METH_NOARGS },
	{ "getPosition", (PyCFunction)PyGetPosition, METH_NOARGS },
	{ NULL, NULL }
};

PyTypeObject _PyCommunicator::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.communicator", /* tp_name */
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