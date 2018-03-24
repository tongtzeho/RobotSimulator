#pragma once

#include "IActionController.h"
#include "Communicator.h"

// ÐèÒªepuck2.lua
class EpuckActionController : public IActionController
{
private:
	CE::Collider *colliderOnGround;
	CE::Rigidbody *rigidbody;
	CE::Entity *wheel;
	Communicator *communicator;
	float maxVelocity;
	float maxOmega;
	EpuckActionController(CE::IComponent *const comp, const void *param = nullptr);
public:
	static IActionController *Instancing(CE::IComponent *const comp, const void *param = nullptr);
	virtual inline const char *GetActionControllerName() const { return "EpuckActionRenderer"; }
	virtual bool IsOutOfControl() const;
	virtual bool IsCrashed() const;
	virtual void Action(const float dt) const;
};