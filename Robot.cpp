#include "Robot.h"

using namespace CE;
using namespace DirectX;

Robot::Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const Transform &transform, CE::Script **const scr)
	: velocity(velocity_), angularVelocity(angularVelocity_), Drawable<>(resourceList, transform, scr) {}

unsigned Robot::StepScripts(const float dt, void *const param)
{
	CheckStarted();
	unsigned ret = 0;
	for (unsigned i = 0; i < MAX_SCRIPT_PER_OBJ; ++i)
	{
		if (scripts[i] != NULL)
		{
			ret |= scripts[i]->Step(dt, param);
		}
	}
	return ret;
}

unsigned Robot::Step(const float dt, void *const param)
{
	unsigned actionCode = StepScripts(dt, param);
	if (actionCode & 0xC)
	{
		if (actionCode & 0x8)
		{
			Turn(true, dt); // Turn Left
		}
		else
		{
			Turn(false, dt); // Turn Right
		}
	}
	if (actionCode & 0x3)
	{
		if (actionCode & 0x2)
		{
			Move(true, dt); // Move Forward
		}
		else
		{
			Move(false, dt);
		}
	}
	return 0;
}

void Robot::Move(const bool forward, const float dt)
{
	Translate(Vector3(0, 0, forward ? 1.0f : -1.0f)*GetQuaternion()*velocity*dt);
}

void Robot::Turn(const bool left, const float dt)
{
	Rotate(Quaternion(0, left ? -sin(angularVelocity*dt / 2) : sin(angularVelocity*dt / 2), 0, cos(angularVelocity*dt / 2)));
}

EPuck::EPuck(const CE::Quaternion &quaternion, const CE::Vector3 &translation)
	: Robot(6.0f, 1.0f, { "@epuck.lua:Mesh", "@epuck.png", "@epuck.lua:Material" }, Transform(Vector3(1, 1, 1), quaternion, translation)), camera(NULL), wheel(NULL)
{
	wheel = new Drawable<>({ "@epuck-wheel.lua:Mesh", "@epuck.png", "@epuck-wheel.lua:Material" }, Transform(Vector3(1, 1, 1), Quaternion(0, 0, 0, 1), Vector3(0, 0.8f, 0)), NULL);
	AddGameObject(wheel);
	camera = new CameraSensor(640, 480, Transform(Vector3(1, 1, 1), Quaternion(0, 0, 0, 1), Vector3(0, 1.14f, 1.39f)), NULL, true, 0.05f, 500.0f, 0.33333333333f * Pi, 100.0f);
	AddGameObject(camera);
}

EPuck::~EPuck()
{
	wheel = NULL;
	camera = NULL;
}

void EPuck::Move(const bool forward, const float dt)
{
	Robot::Move(forward, dt);
	if (forward)
	{
		wheel->Rotate(Quaternion(sin(velocity*dt / 1.6f), 0, 0, cos(velocity*dt / 1.6f)));
	}
	else
	{
		wheel->Rotate(Quaternion(sin(-velocity*dt / 1.6f), 0, 0, cos(-velocity*dt / 1.6f)));
	}
}

void EPuck::Render(const unsigned stage, void *const param)
{
	const unsigned transparentIndex = 2367;
	const float blendFactor[] = { 0.7f, 0.7f, 0.7f, 0.7f };
	CheckStarted();
	if (__drawableActivated && __mesh != NULL && __texture != NULL && __material != NULL && (stage != 0 || __castShadow))
	{
		ID3D11DeviceContext *md3dImmediateContext = CoolEngineGame::Instance()->GetDeviceContext();
		RenderManager *renderMgr = CoolEngineGame::Instance()->GetRenderManager();
		Effect *effect = renderMgr->GetEffectAt(stage == 0 ? 2 : 0); // 默认Stage为0表示阴影，1表示正常
		md3dImmediateContext->IASetInputLayout(effect->GetInputLayout());
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		unsigned stride = sizeof(Vertex32);
		unsigned offset = 0;
		md3dImmediateContext->IASetVertexBuffers(0, 1, __mesh->GetVertexBuffer(), &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(__mesh->GetIndexBuffer(), __mesh->IsIndexByte4() ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
		effect->SetMatrix(XMLoadFloat4x3(&__worldMatrix), 0);
		if (stage == 0)
		{
			effect->SetShaderResource(__texture->GetShaderResourceView(), 0);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetBlendStateAt(-1);
			renderMgr->SetDepthStencilStateAt(-1);
			ID3DX11EffectTechnique *activeTech = effect->GetActiveTech();
			activeTech->GetPassByIndex(0)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(__mesh->GetIndexCount(), 0, 0);
		}
		else
		{
			effect->SetMatrix(__worldMatrix.GetInverseTranspose(), 1);
			effect->SetRawValue(__material, sizeof(Material), 0);
			effect->SetShaderResource(__texture->GetShaderResourceView(), 0);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetBlendStateAt(-1);
			renderMgr->SetDepthStencilStateAt(-1);
			bool shadowTech = CoolEngineGame::Instance()->GetCurrentScene()->IsCastShadow() && __receiveShadow;
			effect->ParseParam(&shadowTech);
			ID3DX11EffectTechnique *activeTech = effect->GetActiveTech();
			activeTech->GetPassByIndex(0)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(transparentIndex, 0, 0);
			renderMgr->SetBlendStateAt(2, blendFactor, 0xFFFFFFFF);
			md3dImmediateContext->DrawIndexed(__mesh->GetIndexCount() - transparentIndex, transparentIndex, 0);
		}
	}
}