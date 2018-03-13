#pragma once

#include "../Engine/CoolEngine.h"

#include "ICameraSensor.h"
#include "IActionController.h"
#include "CommunicationSystem.h"
#include "ActionSystem.h"

class RobotSimulatorScene : public CE::Scene
{
private:
	virtual void RenderObjects(void *const param = nullptr);
	CE::UnstableHashArray<ICameraSensor*> cameraSensorArray;
	CommunicationSystem communicationSys;
	ActionSystem actionSys;
public:
	RobotSimulatorScene() = default;
	virtual ~RobotSimulatorScene() = default;
	virtual void Step(const float dt, void *const param = nullptr);
	virtual void PreRender(void *const param = nullptr);
	virtual void PostRender(void *const param = nullptr);
	bool AppendCameraSensor(ICameraSensor *const cameraSensor);
	bool DeleteCameraSensor(ICameraSensor *const cameraSensor);
	inline CommunicationSystem& GetCommunicationSystem() { return communicationSys; }
	inline ActionSystem& GetActionSystem() { return actionSys; }
};