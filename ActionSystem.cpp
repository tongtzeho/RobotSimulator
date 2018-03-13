#include "ActionSystem.h"

bool ActionSystem::Append(IActionController *const actionController)
{
	assert(actionController != nullptr);
	bool ret = UnstableHashArray<IActionController*>::Append(actionController);
	assert(ret);
	unsafe.reserve(Size());
	return ret;
}

bool ActionSystem::Delete(IActionController *const actionController)
{
	assert(actionController != nullptr);
	bool ret = UnstableErase(actionController);
	assert(ret);
	return ret;
}

void ActionSystem::Action(const float dt) const
{
	for (size_t i = 0; i < Size(); ++i)
	{
		(*this)[i]->Action(dt);
	}
}

const std::vector<IActionController*> &ActionSystem::SafetyVerification()
{
	unsafe.clear();
	for (size_t i = 0; i < Size(); ++i)
	{
		if ((*this)[i]->IsOutOfControl() || (*this)[i]->IsCrashed())
		{
			unsafe.push_back((*this)[i]);
		}
	}
	return unsafe;
}