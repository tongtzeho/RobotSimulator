#include "ProximitySensor.h"

using namespace CE;

bool ProximitySensor::IsNear() const
{
	float distance;
	return GetDistance(distance);
}

template<> Component<ProximitySensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	data = new ProximitySensor(this, identifier);
}

template<> Component<ProximitySensor>::~Component()
{
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<ProximitySensor>::GetTypeName() const
{
	return "ProximitySensor";
}