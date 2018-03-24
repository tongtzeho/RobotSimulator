#include "RGBSensor.h"
#include "RobotSimulatorScene.h"

using namespace CE;

void RobotSimulatorScene::RenderObjects(void *const param)
{
	static ID3DX11Effect *renderEffect = CoolEngine::Instance()->GetRenderManager()->GetEffectByName("$Epuck");
	static ID3DX11EffectVariable* renderEffectLights = renderEffect->GetVariableByName("gLights");
	static ID3DX11EffectScalarVariable* renderEffectLightCount = renderEffect->GetVariableByName("gLightCount")->AsScalar();
	static ID3DX11EffectVectorVariable* renderEffectCameraPos = renderEffect->GetVariableByName("gCameraPos")->AsVector();
	static ID3DX11EffectMatrixVariable* renderEffectViewProj = renderEffect->GetVariableByName("gViewProj")->AsMatrix();
	static ID3DX11EffectMatrixVariable* renderEffectLightViewProj = renderEffect->GetVariableByName("gLightViewProj")->AsMatrix();
	static ID3DX11EffectScalarVariable* renderEffectSceneCastShadow = renderEffect->GetVariableByName("gSceneCastShadow")->AsScalar();
	static ID3DX11EffectShaderResourceVariable* renderEffectShadowMap = renderEffect->GetVariableByName("gShadowMap")->AsShaderResource();

	RenderManager *renderMgr = CoolEngine::Instance()->GetRenderManager();
	Vector3 cameraPos(__currCamera->__GetWorldPosition());
	if (!__lights.empty())
	{
		renderEffectLights->SetRawValue(&__lights[0], 0, sizeof(__Light)*__lights.size());
	}
	if (__mainLight != nullptr)
	{
		renderEffectShadowMap->SetResource(renderMgr->GetShadowMapDepthSRV()); // ÒõÓ°ÌùÍ¼
		renderEffectLightViewProj->SetMatrix(__mainLightViewProj.m[0]);
	}
	renderEffectSceneCastShadow->SetBool(__mainLight != nullptr);
	renderEffectLightCount->SetInt(static_cast<int>(__lights.size()));
	renderEffectViewProj->SetMatrix(__currCamera->__GetViewProjMatrix().m[0]);
	renderEffectCameraPos->SetRawValue(&cameraPos, 0, sizeof(Vector3));

	Scene::RenderObjects(param);
}

void RobotSimulatorScene::Step(const float dt, void *const param)
{
	assert(state == ReadyToUpdate || state == ReadyToRender || state == ReadyToDraw || state == ReadyToPostRender);
	communicationSys.UpdatePositions();
	for (size_t i = 0; i < scriptArray.Size(); ++i)
	{
		scriptArray[i]->Step(dt, (const char*)param);
	}
	actionSys.Action(dt);
	CoolEngine::Instance()->GetPhysicsManager()->Step(dt);
	for (size_t i = 0; i < imuSensorArray.Size(); ++i)
	{
		imuSensorArray[i]->Update(dt);
	}
	actionSys.SafetyVerification();
}

void RobotSimulatorScene::PreRender(void *const param)
{
	for (size_t i = 0; i < cameraSensorArray.Size(); ++i)
	{
		cameraSensorArray[i]->PreRender();
	}
	Scene::PreRender(param);
}

void RobotSimulatorScene::PostRender(void *const param)
{
	assert(state == ReadyToPostRender);
	for (size_t i = 0; i < cameraSensorArray.Size(); ++i)
	{
		cameraSensorArray[i]->Sample();
	}
	state = ReadyToUpdate;
}

bool RobotSimulatorScene::AppendIMUSensor(IMUSensor *const imuSensor)
{
	assert(imuSensor != nullptr);
	bool ret = imuSensorArray.Append(imuSensor);
	assert(ret);
	return ret;
}

bool RobotSimulatorScene::DeleteIMUSensor(IMUSensor *const imuSensor)
{
	assert(imuSensor != nullptr);
	bool ret = imuSensorArray.UnstableErase(imuSensor);
	assert(ret);
	return ret;
}

bool RobotSimulatorScene::AppendCameraSensor(ICameraSensor *const cameraSensor)
{
	assert(cameraSensor != nullptr);
	bool ret = cameraSensorArray.Append(cameraSensor);
	assert(ret);
	return ret;
}

bool RobotSimulatorScene::DeleteCameraSensor(ICameraSensor *const cameraSensor)
{
	assert(cameraSensor != nullptr);
	bool ret = cameraSensorArray.UnstableErase(cameraSensor);
	assert(ret);
	return ret;
}