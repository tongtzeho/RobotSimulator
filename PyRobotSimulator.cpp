#include "RGBSensor.h"
#include "DepthSensor.h"

#include "PyCameraSensor.h"
#include "PyDistanceSensor.h"
#include "PyProximitySensor.h"
#include "PyIMUSensor.h"
#include "PyActionController.h"
#include "PyCommunicator.h"
#include "PyRobotSimulator.h"

#include "RobotSimulatorScene.h"

using namespace CE;

bool PyRobotSimulator::initialized = false;

extern "C"
{
	PyObject* PyRobotSimulator::PyRGBSensor(PyObject *self, PyObject* args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return NULL;
		}
		Component<RGBSensor> *compRGBSensor = dynamic_cast<Component<RGBSensor>*>(pyComp->component);
		if (compRGBSensor == nullptr || **compRGBSensor == nullptr)
		{
			return NULL;
		}
		PyCameraSensor *pyCameraSensor = (PyCameraSensor*)_PyObject_New(const_cast<PyTypeObject*>(PyCameraSensor::GetPyClassInfo()));
		pyCameraSensor->cameraSensor = (RGBSensor*)**compRGBSensor;
		PyObject *ret = Py_BuildValue("O", pyCameraSensor);
		Py_XDECREF(pyCameraSensor);
		return ret;
	}

	PyObject* PyRobotSimulator::PyDepthSensor(PyObject *self, PyObject* args)
	{
		PyComponent *pyComp;
		if (!PyArg_ParseTuple(args, "O", &pyComp))
		{
			return NULL;
		}
		Component<DepthSensor> *compDepthSensor = dynamic_cast<Component<DepthSensor>*>(pyComp->component);
		if (compDepthSensor == nullptr || **compDepthSensor == nullptr)
		{
			return NULL;
		}
		PyCameraSensor *pyCameraSensor = (PyCameraSensor*)_PyObject_New(const_cast<PyTypeObject*>(PyCameraSensor::GetPyClassInfo()));
		pyCameraSensor->cameraSensor = (DepthSensor*)**compDepthSensor;
		PyObject *ret = Py_BuildValue("O", pyCameraSensor);
		Py_XDECREF(pyCameraSensor);
		return ret;
	}

	PyObject* PyRobotSimulator::PyGetUnsafe(PyObject *self)
	{
		RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
		if (simScene != nullptr)
		{
			const std::vector<IActionController*> &unsafe = simScene->GetActionSystem().GetUnsafeResult();
			PyObject *pyList;
			pyList = PyList_New(unsafe.size());
			for (size_t i = 0; i < unsafe.size(); ++i)
			{
				PyEntity *pyEntity = (PyEntity*)_PyObject_New(const_cast<PyTypeObject*>(PyEntity::GetPyClassInfo()));
				pyEntity->entity = unsafe[i]->GetComponent()->GetEntity();
				PyObject *item = Py_BuildValue("O", pyEntity);
				Py_XDECREF(pyEntity);
				PyList_SetItem(pyList, i, item);
			}
			return pyList;
		}
		else
		{
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
}

PyMethodDef PyRobotSimulator::pyMethods[] = {
	{ "rgbSensor", (PyCFunction)PyRGBSensor, METH_VARARGS },
	{ "depthSensor", (PyCFunction)PyDepthSensor, METH_VARARGS },
	{ "getUnsafe", (PyCFunction)PyGetUnsafe, METH_NOARGS },
	{ NULL, NULL }
};

void PyRobotSimulator::Initialize()
{
	assert(!initialized);
	initialized = true;

	PyObject *simModule = Py_InitModule("robotsimulator", pyMethods);
	assert(simModule != NULL);

	int ready = PyType_Ready(const_cast<PyTypeObject*>(PyCameraSensor::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyCameraSensor::GetPyClassInfo()));

	ready = PyType_Ready(const_cast<PyTypeObject*>(PyDistanceSensor::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyDistanceSensor::GetPyClassInfo()));
	PyModule_AddObject(simModule, "distanceSensor", (PyObject*)const_cast<PyTypeObject*>(PyDistanceSensor::GetPyClassInfo()));

	ready = PyType_Ready(const_cast<PyTypeObject*>(PyProximitySensor::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyProximitySensor::GetPyClassInfo()));
	PyModule_AddObject(simModule, "proximitySensor", (PyObject*)const_cast<PyTypeObject*>(PyProximitySensor::GetPyClassInfo()));

	ready = PyType_Ready(const_cast<PyTypeObject*>(PyIMUSensor::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyIMUSensor::GetPyClassInfo()));
	PyModule_AddObject(simModule, "imuSensor", (PyObject*)const_cast<PyTypeObject*>(PyIMUSensor::GetPyClassInfo()));

	ready = PyType_Ready(const_cast<PyTypeObject*>(PyActionController::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyActionController::GetPyClassInfo()));
	PyModule_AddObject(simModule, "actionController", (PyObject*)const_cast<PyTypeObject*>(PyActionController::GetPyClassInfo()));

	ready = PyType_Ready(const_cast<PyTypeObject*>(PyCommunicator::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyCommunicator::GetPyClassInfo()));
	PyModule_AddObject(simModule, "communicator", (PyObject*)const_cast<PyTypeObject*>(PyCommunicator::GetPyClassInfo()));

	PyRun_SimpleString("import robotsimulator");
}