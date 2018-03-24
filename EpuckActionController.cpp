#include "EpuckActionController.h"

using namespace CE;

EpuckActionController::EpuckActionController(IComponent *const comp, const void *param) : IActionController(comp, 3), colliderOnGround(nullptr), rigidbody(nullptr), wheel(nullptr), communicator(nullptr)
{
	Component<Collider> *compColliderOnGround = dynamic_cast<Component<Collider>*>(Entity::GetComponent<Collider>(comp->GetEntity(), "@epuck2.lua:Collider_OnGround"));
	if (compColliderOnGround != nullptr)
	{
		colliderOnGround = (Collider*)**compColliderOnGround;
	}
	Component<Rigidbody> *compRigidbody = dynamic_cast<Component<Rigidbody>*>(Entity::GetComponent<Rigidbody>(comp->GetEntity()));
	if (compRigidbody != nullptr)
	{
		rigidbody = (Rigidbody*)**compRigidbody;
	}
	Component<Communicator> *compCommunicator = dynamic_cast<Component<Communicator>*>(Entity::GetComponent<Communicator>(comp->GetEntity()));
	if (compCommunicator != nullptr)
	{
		communicator = (Communicator*)**compCommunicator;
	}
	wheel = comp->GetEntity()->FindChildEntity("Epuck2Wheel");
	int ret = sscanf((const char*)param, "%f %f", &maxVelocity, &maxOmega);
	assert(ret == 2);
}

IActionController *EpuckActionController::Instancing(IComponent *const comp, const void *param)
{
	return new EpuckActionController(comp, param);
}

bool EpuckActionController::IsOutOfControl() const
{
	if (abs(GetComponent()->GetEntity()->GetEulerAngle().z) >= 0.05f)
	{
		return true;
	}
	else
	{
		std::vector<const CollisionDetectionResult::Manifold*> manifolds;
		CoolEngine::Instance()->GetPhysicsManager()->QueryWorld(colliderOnGround, manifolds);
		return manifolds.size() <= 1; // 除了本身之外，如果底盘还有与其他物体碰撞（如地面），则认为没有失控
	}
}

bool EpuckActionController::IsCrashed() const
{
	std::vector<const CollisionDetectionResult::Manifold*> manifolds;
	CoolEngine::Instance()->GetPhysicsManager()->QueryWorld(rigidbody, manifolds);
	return manifolds.size() >= 3; // 除了底盘的碰撞探测器和接触的地面/平台外，如果还有与其他物体碰撞，则认为发生了碰撞
}

void EpuckActionController::Action(const float dt) const
{
	const float friction = 100.0f;
	const float wheelDiameter = 4.1f;
	if (!IsOutOfControl()) // 没有失控（在平台或地面上）的情况下，可以用行动控制器的运动参数控制机器人，否则由物理引擎当作一般的刚体进行动力学模拟
	{
		Vector3 tangentDir(Vector3(0, 0, 1)*GetComponent()->GetEntity()->GetRotationMatrix());
		Vector3 velocityW(rigidbody->GetVelocity());
		float velocityTangentScalar = velocityW.Dot(tangentDir);
		Vector3 velocityNormal(velocityW - tangentDir*velocityTangentScalar);
		float velocityNormalScalar = velocityNormal.Magnitude();
		if (abs(velocityNormalScalar) > 0.01f)
		{
			Vector3 normalDir(velocityNormal.Unit());
			velocityNormalScalar = max(0, velocityNormalScalar - friction * dt);
			velocityNormal = normalDir*velocityNormalScalar;
		}
		const float velocity = Math::Clamp(GetActionState(0), -maxVelocity, maxVelocity);
		velocityTangentScalar = max(velocity, velocityTangentScalar - friction * dt);
		rigidbody->SetVelocity(tangentDir*velocityTangentScalar + velocityNormal);
		wheel->Rotate(Quaternion(sin(velocityTangentScalar*dt / wheelDiameter), 0, 0, cos(velocityTangentScalar*dt / wheelDiameter)));
		Vector3 omega(rigidbody->GetOmega());
		omega.x = omega.z = 0;
		omega.y = Math::Clamp(GetActionState(1), -maxOmega, maxOmega);
		rigidbody->SetOmega(omega);
	}
	if (GetActionState(2) != 0.0f)
	{
		communicator->Send("1");
	}
}