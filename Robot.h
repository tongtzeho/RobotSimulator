#pragma once

#include "../Engine/Scene/Actor.h"
#include "CameraSensor.h"

class Robot : public CE::Drawable<>
{
protected:
	float velocity;
	float angularVelocity;
	Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const Transform &transform = Transform(), CE::Script **const scr = NULL);
	virtual unsigned StepScripts(const float dt, void *const param = NULL);
	virtual void Move(const bool forward, const float dt);
	virtual void Turn(const bool left, const float dt);
public:
	~Robot() {}
	virtual unsigned Step(const float dt, void *const param = NULL);
};

class EPuck : public Robot
{
private:
	Drawable<> *wheel;
	CameraSensor *camera;
protected:
	virtual void Move(const bool forward, const float dt);
public:
	EPuck(const CE::Quaternion &quaternion = CE::Quaternion(), const CE::Vector3 &translation = CE::Vector3());
	~EPuck();
	virtual void Render(const unsigned stage, void *const param = NULL);
	inline CameraSensor* GetCameraSensor() const { return camera; }
};