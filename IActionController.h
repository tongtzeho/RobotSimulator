#pragma once

#include "../Engine/CoolEngine.h"

class IActionController
{
private:
	CE::IComponent *const component;
	unsigned actionState;
	IActionController(const IActionController&) = delete;
	IActionController& operator=(const IActionController&) = delete;
protected:
	IActionController(CE::IComponent *const comp) : component(comp) {}
public:
	virtual ~IActionController() = default;
	inline CE::IComponent* GetComponent() const { return component; }
	virtual inline const char *GetActionControllerName() const = 0; // 返回具体的行动控制器名称（如"EpuckActionController"）
	virtual bool IsOutOfControl() const = 0;
	virtual bool IsCrashed() const = 0;
	virtual void Action(const float dt) const = 0; // 把行动状态具体地施加到Entity和Entity的其他Component上
	inline unsigned GetActionState() const { return actionState; }
	inline void SetActionState(const unsigned &actionState) { this->actionState = actionState; }
};