#pragma once

#include "../Engine/CoolEngine.h"

#include "IActionController.h"

class ActionSystem : private CE::UnstableHashArray<IActionController*>
{
private:
	std::vector<IActionController*> unsafe;
public:
	bool Append(IActionController *const actionController);
	bool Delete(IActionController *const actionController);
	void Action(const float dt) const;
	const std::vector<IActionController*> &SafetyVerification();
	inline const std::vector<IActionController*> &GetUnsafeResult() const { return unsafe; }
};