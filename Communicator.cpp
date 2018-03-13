#include "Communicator.h"
#include "RobotSimulatorScene.h"

using namespace CE;

void Communicator::UpdatePosition()
{
	Matrix4x3 matrixWorld(component->GetEntity()->GetWorld<Matrix4x3>());
	position = Vector3(matrixWorld._41, matrixWorld._42, matrixWorld._43);
}

void Communicator::Send(const char *msg)
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->GetCommunicationSystem().Send(this, msg);
	}
}

void Communicator::Read(std::vector<std::string> &outMessages)
{
	outMessages.clear();
	if (!empty())
	{
		outMessages.reserve(size());
		while (!empty())
		{
			outMessages.push_back(front());
			pop();
		}
	}
}

template<> Component<Communicator>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	data = new Communicator(this);
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->GetCommunicationSystem().Append(data);
	}
}

template<> Component<Communicator>::~Component()
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->GetCommunicationSystem().Delete(data);
	}
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<Communicator>::GetTypeName() const
{
	return "Communicator";
}