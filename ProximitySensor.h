#pragma once

#include "../Engine/CoolEngine.h"

#include "DistanceSensor.h"

class ProximitySensor : public DistanceSensor // »ùÓÚDistanceSensor
{
public:
	ProximitySensor(CE::IComponent *comp, const char *maxDistanceStr) : DistanceSensor(comp, maxDistanceStr) {}
	~ProximitySensor() = default;
	bool IsNear() const;
};