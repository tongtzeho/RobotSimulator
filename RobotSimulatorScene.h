#pragma once

#include "../Engine/CoolEngine.h"

#include "RGBSensor.h"

class RobotSimulatorScene : public CE::Scene
{
private:
	virtual void RenderObjects(void *const param = nullptr);
	CE::UnstableHashArray<RGBSensor*> rgbSensorArray;
public:
	RobotSimulatorScene() = default;
	virtual ~RobotSimulatorScene() = default;
	virtual void PostRender(void *const param = nullptr);
	bool AppendRGBSensor(RGBSensor *rgbSensor);
	bool DeleteRGBSensor(RGBSensor *rgbSensor);
};