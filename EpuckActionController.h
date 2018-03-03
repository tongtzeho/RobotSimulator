#pragma once

#include "IActionController.h"

class EpuckActionController : public IActionController // 最低2位表示前进速度（4档）。0x8表示有旋转（0x8表示左转，0xC表示右转）
{
private:
	CE::Rigidbody *rigidbody;
	CE::Entity *wheel;
	float velocity;
	float omega;
	EpuckActionController(CE::IComponent *const comp, const void *param = nullptr);
public:
	static IActionController *Instancing(CE::IComponent *const comp, const void *param = nullptr);
	virtual inline const char *GetActionControllerName() const { return "EpuckActionRenderer"; }
	virtual void Action(const float dt) const;
};