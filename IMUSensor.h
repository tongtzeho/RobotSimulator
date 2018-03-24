#pragma once

#include "../Engine/CoolEngine.h"

class IMUSensor
{
private:
	CE::IComponent *const component;
	CE::Rigidbody *rigidbody;
	CE::Vector3 velocity;
	CE::Vector3 acceleration;
public:
	IMUSensor(CE::IComponent *const comp);
	~IMUSensor() = default;
	void Update(const float dt);
	inline const CE::Vector3& GetAcceleration() const { return acceleration; }
	inline CE::IComponent* GetComponent() const { return component; }
};