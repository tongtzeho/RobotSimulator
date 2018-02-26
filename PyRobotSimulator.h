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
	static PyMethodDef pyMethods[];
public:
	static void Initialize();
};