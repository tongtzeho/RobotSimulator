#pragma once

#include "../Engine/CoolEngine.h"

#include "ICameraSensor.h"
#include "IActionController.h"

class RobotSimulatorScene : public CE::Scene
{
private:
	virtual void RenderObjects(void *const param = nullptr);
	CE::UnstableHashArray<ICameraSensor*> cameraSensorArray;
	CE::UnstableHashArray<IActionController*> actionControllerArray;
public:
	RobotSimulatorScene() = default;
	virtual ~RobotSimulatorScene() = default;
	virtual void Step(const float dt, void *const param = nullptr);
	virtual void PreRender(void *const param = nullptr);
	virtual void PostRender(void *const param = nullptr);
	bool AppendCameraSensor(ICameraSensor *const cameraSensor);
	bool DeleteCameraSensor(ICameraSensor *const cameraSensor);
	bool AppendActionController(IActionController *const actionController);
	bool DeleteActionController(IActionController *const actionController);
};