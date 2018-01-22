#pragma once

#include "../Engine/Scene/Renderable.h"
#include "CameraSensor.h"

class Robot : public CE::Renderable<>
{
protected:
	float velocity;
	float angularVelocity;
	Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const Transform &transform = Transform(), CE::Script **const scr = nullptr);
	virtual unsigned StepScripts(const float dt, void *const param = nullptr);
	virtual void Move(const bool forward, const float dt);
	virtual void Turn(const bool left, const float dt);
public:
	~Robot() {}
	virtual unsigned Step(const float dt, void *const param = nullptr);
};

class EPuck : public Robot
{
private:
	Renderable<> *wheel;
	CameraSensor *camera;
protected:
	virtual void Move(const bool forward, const float dt);
public:
	EPuck(const CE::Quaternion &quaternion = CE::Quaternion(), const CE::Vector3 &translation = CE::Vector3());
	~EPuck();
	virtual void Render(const unsigned stage, void *const param = nullptr);
	inline CameraSensor* GetCameraSensor() const { return camera; }
};