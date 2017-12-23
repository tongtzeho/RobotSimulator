#pragma once

#include "../Engine/Scene/Actor.h"

class Robot : public CE::Actor<>
{
protected:
	CE::Material shadowMaterial;
	float velocity;
	float angularVelocity;
	Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const std::unordered_map<std::string, std::string> &animResourceMap, const Transform &transform = Transform(), CE::Script **const scr = NULL);
	virtual unsigned FixedUpdateScripts(const float dt, void *const param = NULL);
	virtual void Move(const bool forward, const float dt);
	virtual void Turn(const bool left, const float dt);
public:
	~Robot() {}
	virtual unsigned FixedUpdate(const float dt, void *const param = NULL);
};

class EPuck : public Robot
{
public:
	EPuck(const CE::Quaternion &quaternion = CE::Quaternion(), const CE::Vector3 &translation = CE::Vector3());
};