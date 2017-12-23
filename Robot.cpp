#include "Robot.h"

using namespace CE;
using namespace DirectX;

Robot::Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const std::unordered_map<std::string, std::string> &animResourceMap, const Transform &transform, CE::Script **const scr)
	: velocity(velocity_), angularVelocity(angularVelocity_), Actor<>(resourceList, animResourceMap, "idle", transform, scr)
{
	shadowMaterial.ambient.w = 1;
	shadowMaterial.diffuse.w = 0.3f;
	shadowMaterial.specular.w = 16;
}

unsigned Robot::FixedUpdateScripts(const float dt, void *const param)
{
	CheckStarted();
	unsigned ret = 0;
	for (unsigned i = 0; i < MAX_SCRIPT_PER_OBJ; ++i)
	{
		if (scripts[i] != NULL)
		{
			ret |= scripts[i]->FixedUpdate(dt, param);
		}
	}
	return ret;
}

unsigned Robot::FixedUpdate(const float dt, void *const param)
{
	unsigned actionCode = FixedUpdateScripts(dt, param);
	if (actionCode & 0xC)
	{
		if (actionCode & 0x8)
		{
			Turn(true, dt); // Turn Left
		}
		else
		{
			Turn(false, dt); // Turn Right
		}
	}
	if (actionCode & 0x3)
	{
		if (actionCode & 0x2)
		{
			Move(true, dt); // Move Forward
		}
		else
		{
			Move(false, dt);
		}
	}
	else
	{
		SetAnimatorState("idle");
	}
	return 0;
}

void Robot::Move(const bool forward, const float dt)
{
	Translate(Vector3(0, 0, forward ? 1.0f : -1.0f)*GetQuaternion()*velocity*dt);
	SetAnimatorState("move");
}

void Robot::Turn(const bool left, const float dt)
{
	Rotate(Quaternion(0, left ? -sin(angularVelocity*dt / 2) : sin(angularVelocity*dt / 2), 0, cos(angularVelocity*dt / 2)));
}

EPuck::EPuck(const CE::Quaternion &quaternion, const CE::Vector3 &translation)
	: Robot(50.0f, 1.0f, { "@epuck.skn:M", "@epuck.dds", "@epuck.lua:Material", "@epuck.skn", "@epuck.skl" }, { { "idle", "@epuck_idle.anm" },{ "move", "@epuck_move.anm" },{ "dance", "@epuck_dance.anm" } }, Transform(Vector3(0.5f, 0.5f, 0.5f), quaternion, translation)) {}