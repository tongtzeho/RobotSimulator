#include "PyActionController.h"

using namespace CE;

extern "C"
{
	PyObject* _PyActionController::PyNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyActionController *self = (PyActionController*)type->tp_alloc(type, 0);
		self->actionController = nullptr;
		return (PyObject*)self;
	}

	int _PyActionController::PyInit(_PyActionController *self, PyObject *args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return -1;
		}
		Component<IActionController> *compIActionRenderer = dynamic_cast<Component<IActionController>*>(pyComp->component);
		if (compIActionRenderer == nullptr)
		{
			return -1;
		}
		self->actionController = (IActionController*)**compIActionRenderer;
		if (self->actionController == nullptr)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	void _PyActionController::PyDel(_PyActionController *self)
	{
		return;
	}

	PyObject* _PyActionController::PyStr(_PyActionController *self)
	{
		char str[64] = { 0 };
		sprintf(str, "&%s@0x%llX", self->actionController->GetActionControllerName(), reinterpret_cast<long long>(self->actionController));
		return PyString_FromString(str);
	}

	PyObject* _PyActionController::PyRepr(_PyActionController *self)
	{
		return PyStr(self);
	}

	PyObject* _PyActionController::PyGetState(_PyActionController *self, PyObject *args)
	{
		unsigned n;
		if (!PyArg_ParseTuple(args, "I", &n))
		{
			return NULL;
		}
		return PyFloat_FromDouble(self->actionController->GetActionState(n));
	}

	PyObject* _PyActionController::PyGetStates(_PyActionController *self)
	{
		const std::vector<float> &states = self->actionController->GetActionState();
		PyObject *pyList;
		pyList = PyList_New(states.size());
		for (size_t i = 0; i < states.size(); ++i)
		{
			PyList_SetItem(pyList, i, PyFloat_FromDouble(states[i]));
		}
		return pyList;
	}

	PyObject* _PyActionController::PySetState(_PyActionController *self, PyObject *args)
	{
		unsigned n;
		float state;
		if (!PyArg_ParseTuple(args, "If", &n, &state))
		{
			return NULL;
		}
		self->actionController->SetActionState(n, state);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* _PyActionController::PySetStates(_PyActionController *self, PyObject *args)
	{
		PyObject *list;
		if (!PyArg_ParseTuple(args, "O", &list))
		{
			return NULL;
		}
		size_t n = PyList_Size(list);
		std::vector<float> states(n, 0);
		for (size_t i = 0; i < n; ++i)
		{
			states[i] = static_cast<float>(PyFloat_AsDouble(PyList_GetItem(list, i)));
		}
		self->actionController->SetActionState(states);
		Py_INCREF(Py_None);
		return Py_None;
	}
}

PyMemberDef _PyActionController::PyDataMembers[] = {
	{ NULL, NULL, NULL, 0, NULL }
};

PyMethodDef _PyActionController::PyMethodMembers[] = {
	{ "getState", (PyCFunction)PyGetState, METH_VARARGS },
	{ "getStates", (PyCFunction)PyGetStates, METH_NOARGS },
	{ "setState", (PyCFunction)PySetState, METH_VARARGS },
	{ "setStates", (PyCFunction)PySetStates, METH_VARARGS },
	{ NULL, NULL }
};

PyTypeObject _PyActionController::PyClassInfo = {
	PyVarObject_HEAD_INIT(NULL, 0)"robotsimulator.actionController", /* tp_name */
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