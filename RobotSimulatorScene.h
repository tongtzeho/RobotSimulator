#pragma once

#include "../Engine/CoolEngine.h"

#include "ICameraSensor.h"

class RobotSimulatorScene : public CE::Scene
{
private:
	virtual void RenderObjects(void *const param = nullptr);
	CE::UnstableHashArray<ICameraSensor*> cameraSensorArray;
public:
	RobotSimulatorScene() = default;
	virtual ~RobotSimulatorScene() = default;
	virtual void PreRender(void *const param = nullptr);
	virtual void PostRender(void *const param = nullptr);
	bool AppendCameraSensor(ICameraSensor *const cameraSensor);
	bool DeleteCameraSensor(ICameraSensor *const cameraSensor);
};