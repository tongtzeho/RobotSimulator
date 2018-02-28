#pragma once

#include "ICameraSensor.h"

class DepthSensor : public ICameraSensor
{
private:
	float farZ; // 深度传感器可探测的最远距离（与Camera的最远距离不同）
	virtual unsigned char* GetBMPPixels() const;
public:
	DepthSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	virtual ~DepthSensor() = default;
	virtual const char *GetSensorType() const { return "DepthSensor"; }
	virtual bool Sample(); // 复制后，重新换算深度值
};