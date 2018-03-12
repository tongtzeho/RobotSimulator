#pragma once

#include "../Engine/CoolEngine.h"

class DistanceSensor
{
private:
	CE::IComponent *const component;
	float maxDistance;
public:
	DistanceSensor(CE::IComponent *const comp, const char *maxDistanceStr);
	~DistanceSensor() = default;
	bool GetDistance(float &outDistance) const;
	inline CE::IComponent* GetComponent() const { return component; }
};