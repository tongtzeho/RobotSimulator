#pragma once

#include "../Engine/CoolEngine.h"

class PyRobotSimulator
{
private:
	PyRobotSimulator() = delete;
	~PyRobotSimulator() = delete;
	PyRobotSimulator(const PyRobotSimulator&) = delete;
	PyRobotSimulator& operator=(const PyRobotSimulator&) = delete;
	void *operator new(size_t) = delete;
	void operator delete(void* ptr) = delete;
	static bool initialized;
	static PyObject* PyRGBSensor(PyObject *self, PyObject* args); // ����һ��PyCameraSensor��ָ���cameraSensor��RGBSensor
	static PyObject* PyDepthSensor(PyObject *self, PyObject* args); // ����һ��PyCameraSensor��ָ���cameraSensor��DepthSensor
	static PyMethodDef pyMethods[];
public:
	static void Initialize();
};