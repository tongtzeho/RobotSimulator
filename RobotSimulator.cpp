#include "Robot.h"
#include "RobotSimulator.h"
#include "DepthMapEffect.h"
#include "GUICameraView.h"
#include "CameraSensor.h"
#include "../Engine/Render/WICTextureLoader.h"

using namespace CE;

RobotSimulatorScene::RobotSimulatorScene() : Scene(NULL, "@background.dds") {}

void RobotSimulatorScene::Start(void *const param)
{
	Scene::Start();
	Camera *freeCamera = new Camera(Transform(Vector3(1, 1, 1), Quaternion(0.5f, 0, 0, 0.866025403784f), Vector3(0, 60, -50)), NULL, true, 1.0f, 10000.0f, 0.333333333f*Pi);
	freeCamera->AddScript(new PythonScript("freeCamera"), false);
	AddGameObject(freeCamera);
	CameraSensor *overLookCamera = new CameraSensor(640, 360, Transform(Vector3(1, 1, 1), Quaternion(0.7071f, 0, 0, 0.707101f), Vector3(0, 10, 0)), NULL, false, 0.5f, 13.0f, 130.0f);
	overLookCamera->SetRenderShadow(false);
	AddGameObject(overLookCamera);
	Drawable<> *floor = new Drawable<>({ "@floor.lua:Mesh", "@floor.dds", "@floor.lua:Material" });
	floor->SetCastShadow(false);
	floor->SetReceiveShadow(false);
	AddGameObject(floor);
	Drawable<> *screen = new Drawable<>({ "@screen.lua:Mesh", "@screen.png", "@screen.lua:Material" });
	screen->SetCastShadow(false);
	AddGameObject(screen);
	Drawable<> *platform = new Drawable<>({ "@platform.lua:Mesh", "@platform.png", "@platform.lua:Material" });
	platform->SetCastShadow(false);
	platform->SetReceiveShadow(false);
	AddGameObject(platform);
	EPuck *epuck = new EPuck(Quaternion(), Vector3(50, 0, 0));
	epuck->AddScript(new PythonScript("kbdControl"), false);
	AddGameObject(epuck);
	lights.push_back(Light());
	lights[0].SetDirectionalLight(Color(0xffffffff), Color(0xc0c0c0ff), Color(0x484848ff), Vector3(0.5f, -0.5f, 0.8f), Vector3(0, 0, 0), 80);
	GUIText *cameraInfo = new GUIText("Camera: Pos = (0, 0, 0) Dir = (0, 0, 0)", FontSheet::FontStyleRegular, 0xff000000, 3, GUITransform(0, 0, 0, 0, 8, 5, 0.35f, 0.35f, 0, 0xC));
	cameraInfo->AddScript(new PythonScript("cameraInfo"), false);
	AddGUIObject(cameraInfo);
	GUICameraView *shadowMap = new GUICameraView(1, GUITransform(1, 1, 1, 1, 0, 0, 400, 225, 0, 0xF));
	shadowMap->SetTexture(overLookCamera->GetDrawableTexture()->GetColorTexture(), false);
	AddGUIObject(shadowMap);
}

void RobotSimulatorScene::PreRender(void *const param)
{
	CheckStarted();
	std::vector<CameraSensor*> sensorList;
	sensorList.reserve(32);
	if (IsCastShadow())
	{
		Effect *effect = CoolEngineGame::Instance()->GetRenderManager()->GetEffectAt(2);
		effect->SetMatrix(XMLoadFloat4x3(&lights[mainLightIndex].GetViewMatrix())*lights[mainLightIndex].GetProjMatrix(), 1);
	}
	for (std::list<GameObject*>::iterator iter = gameObjectList.begin(); iter != gameObjectList.end(); ++iter)
	{
		(*iter)->PreRender(&sensorList);
	}
	for (size_t i = 0; i < sensorList.size(); ++i)
	{
		sensorList[i]->GetDrawableTexture()->Reset();
		int oldMainLightIndex = mainLightIndex;
		if (!sensorList[i]->isRenderShadow())
		{
			mainLightIndex = -1;
		}
		Effect *effect = CoolEngineGame::Instance()->GetRenderManager()->GetEffectAt(0);
		Vector3 cameraPos;
		if (!lights.empty())
		{
			effect->SetRawValue(&lights[0], sizeof(Light)*lights.size(), 1);
		}
		if (IsCastShadow())
		{
			effect->SetShaderResource(CoolEngineGame::Instance()->GetRenderManager()->GetShadowMapDepthSRV(), 1); // ÒõÓ°ÌùÍ¼
			effect->SetMatrix(XMLoadFloat4x3(&lights[mainLightIndex].GetViewMatrix())*lights[mainLightIndex].GetProjMatrix(), 3);
		}
		int lightCount = lights.size();
		effect->SetScalar(&lightCount);
		DirectX::XMMATRIX cameraViewProj = XMLoadFloat4x3(&sensorList[i]->GetViewMatrix(&cameraPos))*sensorList[i]->GetProjMatrix();
		effect->SetMatrix(cameraViewProj, 2);
		effect->SetRawValue(&cameraPos, sizeof(Vector3), 2);
		for (std::list<GameObject*>::iterator iter = gameObjectList.begin(); iter != gameObjectList.end(); ++iter)
		{
			(*iter)->Render(param);
		}
		mainLightIndex = oldMainLightIndex;
	}
}

void RobotSimulatorScene::Draw(void *const param)
{
	CheckStarted();
	std::vector<std::pair<Effect*, unsigned>> shaders = { { CoolEngineGame::Instance()->GetRenderManager()->GetEffectAt(1), 1}, { CoolEngineGame::Instance()->GetRenderManager()->GetEffectAt(4), 1 } };
	CoolEngineGame::Instance()->GetGUIManager()->SetNDCMatrixToShaders(shaders);
	for (std::list<GUIObject*>::iterator iter = guiObjectList.begin(); iter != guiObjectList.end(); ++iter)
	{
		(*iter)->Draw(param);
	}
}

RobotSimulator::RobotSimulator(HINSTANCE hInstance) : CoolEngineGame(hInstance)
{
	mMainWndCaption = L"Robot Simulator";
}

bool RobotSimulator::Init()
{
	if (!CoolEngineGame::Init("RobotSimulator/"))
	{
		return false;
	}

	GetRenderManager()->AddEffect(new DepthMapEffect(GetDevice()));

	// Init Scene
	scene = new RobotSimulatorScene();
	scene->Start();

	return true;
}