#pragma once

#include "../Engine/CoolEngineGame.h"
#include "CameraSensor.h"

class RobotSimulatorScene : public CE::Scene
{
	std::vector<CameraSensor*> cameraSensorList;
public:
	RobotSimulatorScene();
	virtual void Start(void *const param = nullptr);
	virtual void PreRender(void *const param = nullptr);
	virtual void Render(void *const param = nullptr);
	virtual void Draw(void *const param = nullptr);
};

class RobotSimulator : public CE::CoolEngineGame
{
public:
	RobotSimulator(HINSTANCE hInstance);
	bool InitRobotSimulator();
};