#include "IMUSensor.h"
#include "RobotSimulatorScene.h"

using namespace CE;

IMUSensor::IMUSensor(CE::IComponent *const comp) : component(comp), rigidbody(nullptr)
{
	Component<Rigidbody> *compRigidbody = dynamic_cast<Component<Rigidbody>*>(Entity::GetComponent<Rigidbody>(comp->GetEntity()));
	if (compRigidbody != nullptr)
	{
		rigidbody = (Rigidbody*)**compRigidbody;
	}
}

void IMUSensor::Update(const float dt)
{
	Vector3 velocityW(rigidbody->GetVelocity());
	Vector3 velocityL(velocityW*Math::RotationMatrixToQuaternion(component->GetEntity()->GetWorld<Matrix4x3>().GetInverse()));
	acceleration = (velocityL - velocity) / dt;
	velocity = velocityL;
}

template<> Component<IMUSensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	data = new IMUSensor(this);
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->AppendIMUSensor(data);
	}
}

template<> Component<IMUSensor>::~Component()
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->DeleteIMUSensor(data);
	}
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<IMUSensor>::GetTypeName() const
{
	return "IMUSensor";
}