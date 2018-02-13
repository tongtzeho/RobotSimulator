#pragma once

#include "../Engine/CoolEngine.h"

class RobotSimulator : public CE::CoolEngine
{
public:
	RobotSimulator(HINSTANCE hInstance);
	virtual bool Init(const char *rootDir = nullptr);
};