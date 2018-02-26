#include "PyRGBSensor.h"
#include "PyRobotSimulator.h"

bool PyRobotSimulator::initialized = false;

PyMethodDef PyRobotSimulator::pyMethods[] = {
	{ NULL, NULL }
};

void PyRobotSimulator::Initialize()
{
	assert(!initialized);
	initialized = true;

	PyObject *simModule = Py_InitModule("robotsimulator", pyMethods);
	assert(simModule != NULL);

	int ready = PyType_Ready(const_cast<PyTypeObject*>(PyRGBSensor::GetPyClassInfo()));
	assert(ready >= 0);
	Py_INCREF(const_cast<PyTypeObject*>(PyRGBSensor::GetPyClassInfo()));
	PyModule_AddObject(simModule, "rgbSensor", (PyObject*)const_cast<PyTypeObject*>(PyRGBSensor::GetPyClassInfo()));

	PyRun_SimpleString("import robotsimulator");
}