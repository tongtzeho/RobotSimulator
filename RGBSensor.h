#pragma once

#include "ICameraSensor.h"

class RGBSensor : public ICameraSensor
{
private:
	virtual unsigned char* GetBMPPixels() const;
public:
	RGBSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	virtual ~RGBSensor() = default;
	virtual void SendDataToSocketClient(SOCKET clientSocket);
	virtual const char *GetSensorType() const { return "RGBSensor"; }
};