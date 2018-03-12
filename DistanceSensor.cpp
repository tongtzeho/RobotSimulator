#include "DistanceSensor.h"

using namespace CE;

DistanceSensor::DistanceSensor(CE::IComponent *comp, const char *maxDistanceStr) : component(comp)
{
	int ret = sscanf(maxDistanceStr, "%f", &maxDistance);
	assert(ret == 1 && maxDistance > 0);
}

bool DistanceSensor::GetDistance(float &outDistance) const
{
	Matrix4x3 worldMatrix(component->GetEntity()->GetWorld<Matrix4x3>());
	Vector3 start(worldMatrix._41, worldMatrix._42, worldMatrix._43);
	Vector3 dir((Vector3(0, 0, 1)*worldMatrix).Unit());
	Vector3 hit;
	Vector3 normal;
	if (CoolEngine::Instance()->GetPhysicsManager()->RayTest(start, start + dir*maxDistance, hit, normal) != nullptr)
	{
		outDistance = (hit - start).Magnitude();
		return true;
	}
	else
	{
		return false;
	}
}

template<> Component<DistanceSensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	data = new DistanceSensor(this, identifier);
}

template<> Component<DistanceSensor>::~Component()
{
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<DistanceSensor>::GetTypeName() const
{
	return "DistanceSensor";
}