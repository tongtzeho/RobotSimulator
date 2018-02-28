#pragma once

#include "ICameraSensor.h"

class DepthSensor : public ICameraSensor
{
private:
	float farZ; // ��ȴ�������̽�����Զ���루��Camera����Զ���벻ͬ��
	virtual unsigned char* GetBMPPixels() const;
public:
	DepthSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	virtual ~DepthSensor() = default;
	virtual const char *GetSensorType() const { return "DepthSensor"; }
	virtual bool Sample(); // ���ƺ����»������ֵ
};