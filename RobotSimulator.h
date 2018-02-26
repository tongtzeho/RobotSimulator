#pragma once

#include "../Engine/CoolEngine.h"

class RobotSimulator : public CE::CoolEngine
{
private:
	virtual void InitPython(const char *rootDir = nullptr);
public:
	RobotSimulator(HINSTANCE hInstance);
	virtual bool Init(const char *rootDir = nullptr);
};