#pragma once

#include "../Engine/CoolEngineGame.h"
#include "../Engine/Scene/Actor.h"

class RobotSimulatorScene : public CE::Scene
{
public:
	RobotSimulatorScene();
	virtual void Start(void *const param = NULL);
};

class RobotSimulator : public CE::CoolEngineGame
{
public:
	RobotSimulator(HINSTANCE hInstance);
	bool Init();
};