#include "Robot.h"

using namespace CE;
using namespace DirectX;

Robot::Robot(const float velocity_, const float angularVelocity_, const std::vector<std::string> &resourceList, const Transform &transform, CE::Script **const scr)
	: velocity(velocity_), angularVelocity(angularVelocity_), Renderable<>(resourceList, transform, scr) {}

unsigned Robot::StepScripts(const float dt, void *const param)
{
	CheckStarted();
	unsigned ret = 0;
	for (unsigned i = 0; i < MAX_SCRIPT_PER_OBJ; ++i)
	{
		if (scripts[i] != nullptr)
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
	: Robot(6.0f, 1.0f, { "@epuck.lua:Mesh", "@epuck.png", "@epuck.lua:Material" }, Transform(Vector3(1, 1, 1), quaternion, translation)), camera(nullptr), wheel(nullptr)
{
	wheel = new Renderable<>({ "@epuck-wheel.lua:Mesh", "@epuck.png", "@epuck-wheel.lua:Material" }, Transform(Vector3(1, 1, 1), Quaternion(0, 0, 0, 1), Vector3(0, 0.8f, 0)), nullptr);
	AddGameObject(wheel);
	camera = new CameraSensor(640, 480, Transform(Vector3(1, 1, 1), Quaternion(0, 0, 0, 1), Vector3(0, 1.14f, 1.39f)), nullptr, true, 0.05f, 500.0f, 0.33333333333f * Pi, 100.0f);
	AddGameObject(camera);
}

EPuck::~EPuck()
{
	wheel = nullptr;
	camera = nullptr;
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
	if (__renderableActivated && __mesh != nullptr && __texture != nullptr && __material != nullptr)
	{
		Renderable<>::InputAssemblerSetting(stage, param);
		ID3D11DeviceContext *d3d11DeviceContext = CoolEngineGame::Instance()->GetDeviceContext();
		RenderManager *renderMgr = CoolEngineGame::Instance()->GetRenderManager();
		Effect *effect = renderMgr->GetEffectAt(stage == RenderManager::DrawShadowMap ? 2 : 0);
		effect->SetMatrix(XMLoadFloat4x3(&__worldMatrix), 0);
		if (stage == RenderManager::DrawShadowMap)
		{
			effect->SetShaderResource(__texture->GetShaderResourceView(), 0);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetBlendStateAt(-1);
			renderMgr->SetDepthStencilStateAt(-1);
			ID3DX11EffectTechnique *activeTech = effect->ActivateTech(0);
			activeTech->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(__mesh->GetIndexCount(), 0, 0);
		}
		else
		{
			effect->SetMatrix(__worldMatrix.GetInverseTranspose(), 1);
			effect->SetRawValue(__material, sizeof(Material), 0);
			effect->SetShaderResource(__texture->GetShaderResourceView(), 0);
			bool receiveShadow = CoolEngineGame::Instance()->GetCurrentScene()->IsCastShadow() && __receiveShadow;
			effect->SetScalar(&receiveShadow, 2);
			ID3DX11EffectTechnique *activeTech = effect->ActivateTech(0);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetDepthStencilStateAt(-1);
			activeTech->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(transparentIndex, 0, 0);
			renderMgr->SetBlendStateAt(2, blendFactor, 0xFFFFFFFF);
			d3d11DeviceContext->DrawIndexed(__mesh->GetIndexCount() - transparentIndex, transparentIndex, 0);
		}
	}
}