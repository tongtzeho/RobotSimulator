#include "CommunicationSystem.h"
#include "PyCommunicator.h"

using namespace CE;

CommunicationSystem::CommunicationSystem() : pySend(NULL), pyCommList(NULL)
{
	PyRun_SimpleString("import sys");
	PyObject *pyModule = PyImport_ImportModule("communication_system");
	if (pyModule != NULL)
	{
		if (PyObject_HasAttrString(pyModule, "send"))
		{
			pySend = PyObject_GetAttrString(pyModule, "send");
		}
		Py_XDECREF(pyModule);
	}
	UpdatePyCommunicatorList();
}

CommunicationSystem::~CommunicationSystem()
{
	Py_XDECREF(pyCommList);
	if (pySend != NULL)
	{
		PyRun_SimpleString("import sys");
		Py_XDECREF(pySend);
	}
}

bool CommunicationSystem::Append(Communicator *const communicator)
{
	assert(communicator != nullptr);
	bool ret = UnstableHashArray<Communicator*>::Append(communicator);
	assert(ret);
	UpdatePyCommunicatorList();
	return ret;
}

bool CommunicationSystem::Delete(Communicator *const communicator)
{
	assert(communicator != nullptr);
	bool ret = UnstableErase(communicator);
	assert(ret);
	UpdatePyCommunicatorList();
	return ret;
}

void CommunicationSystem::UpdatePyCommunicatorList()
{
	if (pyCommList != NULL)
	{
		Py_XDECREF(pyCommList);
	}
	pyCommList = PyList_New(Size());
	for (size_t i = 0; i < Size(); ++i)
	{
		PyCommunicator *pyCommunicator = (PyCommunicator*)_PyObject_New(const_cast<PyTypeObject*>(PyCommunicator::GetPyClassInfo()));
		pyCommunicator->communicator = (*this)[i];
		PyObject *item = Py_BuildValue("O", pyCommunicator);
		Py_XDECREF(pyCommunicator);
		PyList_SetItem(pyCommList, i, item);
	}
}

PyObject *CommunicationSystem::GetPyCommunicator(Communicator *sender) const
{
	for (size_t i = 0; i < Size(); ++i)
	{
		if (sender == (*this)[i])
		{
			return PyList_GetItem(pyCommList, i);
		}
	}
	return NULL;
}

void CommunicationSystem::UpdatePositions()
{
	for (size_t i = 0; i < Size(); ++i)
	{
		(*this)[i]->UpdatePosition();
	}
}

void CommunicationSystem::Send(Communicator *sender, const char *msg)
{
	if (pySend != NULL)
	{
		PyObject *pySender = GetPyCommunicator(sender);
		if (pySender != NULL)
		{
			PyRun_SimpleString("import sys");
			PyObject *pyArgs = PyTuple_New(3);
			Py_INCREF(pySender);
			PyTuple_SetItem(pyArgs, 0, pySender);
			PyTuple_SetItem(pyArgs, 1, PyString_FromString(msg));
			Py_INCREF(pyCommList);
			PyTuple_SetItem(pyArgs, 2, pyCommList);
			PyObject *ret = PyEval_CallObject(pySend, pyArgs); // 返回值：由tuple(receiver, msg)组成的list
			Py_XDECREF(pyArgs);
			if (ret != NULL)
			{
				int len = PyList_Size(ret);
				for (int i = 0; i < len; ++i)
				{
					PyObject *item = PyList_GetItem(ret, i);
					PyCommunicator *receiver;
					const char *sendMsg;
					double delay;
					if (!PyArg_ParseTuple(item, "Osd", &receiver, &sendMsg, &delay))
					{
						continue;
					}
					if (delay < 0)
					{
						delay = 0;
					}
					receiver->communicator->Receive(sendMsg, CoolEngine::Instance()->GetTime()+delay);
				}
				Py_XDECREF(ret);
			}
		}
	}
}