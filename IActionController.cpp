#include "IActionController.h"
#include "RobotSimulatorScene.h"
#include "RobotSimulator.h"

using namespace CE;

template<> Component<IActionController>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	assert(param != nullptr);
	RobotSimulator *sim = dynamic_cast<RobotSimulator*>(CoolEngine::Instance());
	if (sim != nullptr)
	{
		data = sim->InstancingActionController(identifier, this, param);
		RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
		if (simScene != nullptr)
		{
			simScene->AppendActionController(data);
		}
	}
}

template<> Component<IActionController>::~Component()
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->DeleteActionController(data);
	}
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<IActionController>::GetTypeName() const
{
	return "ActionController";
}