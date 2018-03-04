#include "EpuckActionController.h"

using namespace CE;

EpuckActionController::EpuckActionController(IComponent *const comp, const void *param) : IActionController(comp), colliderOnGround(nullptr), rigidbody(nullptr), wheel(nullptr)
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
	wheel = comp->GetEntity()->FindChildEntity("Epuck2Wheel");
	int ret = sscanf((const char*)param, "%f %f", &velocity, &omega);
	assert(ret == 2);
	SetActionState(0);
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
		return manifolds.size() <= 1; // ���˱���֮�⣬������̻���������������ײ������棩������Ϊû��ʧ��
	}
}

void EpuckActionController::Action(const float dt) const
{
	const float friction = 100.0f;
	const float wheelDiameter = 4.1f;
	if (!IsOutOfControl()) // û��ʧ�أ���ƽ̨������ϣ�������£��������ж����������˶��������ƻ����ˣ��������������浱��һ��ĸ�����ж���ѧģ��
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
		velocityTangentScalar = max(velocity*(GetActionState() & 0x3), velocityTangentScalar - friction * dt);
		rigidbody->SetVelocity(tangentDir*velocityTangentScalar + velocityNormal);
		wheel->Rotate(Quaternion(sin(velocityTangentScalar*dt / wheelDiameter), 0, 0, cos(velocityTangentScalar*dt / wheelDiameter)));
		Vector3 omega(rigidbody->GetOmega());
		omega.x = omega.z = 0;
		if (GetActionState() & 0x8)
		{
			if (GetActionState() & 0x4)
			{
				omega.y = this->omega;
			}
			else
			{
				omega.y = -this->omega;
			}
		}
		else
		{
			omega.y = 0;
		}
		rigidbody->SetOmega(omega);
	}
}