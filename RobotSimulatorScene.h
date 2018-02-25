#pragma once

#include "../Engine/CoolEngine.h"

class RobotSimulatorScene : public CE::Scene
{
private:
	virtual void RenderObjects(void *const param = nullptr);
public:
	RobotSimulatorScene() = default;
	virtual ~RobotSimulatorScene() = default;
};