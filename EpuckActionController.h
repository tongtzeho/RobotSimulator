#pragma once

#include "IActionController.h"

// ��Ҫepuck2.lua
class EpuckActionController : public IActionController // ���2λ��ʾǰ���ٶȣ�4������0x8��ʾ����ת��0x8��ʾ��ת��0xC��ʾ��ת��
{
private:
	CE::Collider *colliderOnGround;
	CE::Rigidbody *rigidbody;
	CE::Entity *wheel;
	float velocity;
	float omega;
	EpuckActionController(CE::IComponent *const comp, const void *param = nullptr);
	bool IsOutOfControl() const;
public:
	static IActionController *Instancing(CE::IComponent *const comp, const void *param = nullptr);
	virtual inline const char *GetActionControllerName() const { return "EpuckActionRenderer"; }
	virtual void Action(const float dt) const;
};