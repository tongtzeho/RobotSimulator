#include "Robot.h"
#include "RobotSimulator.h"
#include "DepthMapEffect.h"
#include "GUICameraView.h"
#include "CameraSensor.h"
#include "Wall.h"
#include "../Engine/Render/WICTextureLoader.h"

using namespace CE;

RobotSimulatorScene::RobotSimulatorScene() : Scene(NULL, "@background.dds", "@splash.png") {}

void RobotSimulatorScene::Start(void *const param)
{
	state = Starting;
	Sleep(300);
	StartScripts(param);
	cameraSensorList.reserve(64);
	Camera *freeCamera = new Camera(Transform(Vector3(1, 1, 1), Quaternion(0.5f, 0, 0, 0.866025403784f), Vector3(0, 60, -50)), NULL, true, 1.0f, 10000.0f, 0.333333333f*Pi);
	freeCamera->AddScript(new PythonScript("freeCamera"), false);
	AddGameObject(freeCamera);
	CameraSensor *overLookCamera = new CameraSensor(640, 360, Transform(Vector3(1, 1, 1), Quaternion(0.7071f, 0, 0, 0.707101f), Vector3(0, 9.9f, 0)), NULL, false, 0.0f, 10.0f, 128.0f, 10.0f);
	overLookCamera->SetRenderShadow(false);
	AddGameObject(overLookCamera);
	Drawable<> *floor = new Drawable<>({ "@floor.lua:Mesh", "@floor.dds", "@floor.lua:Material" });
	floor->SetCastShadow(false);
	floor->SetReceiveShadow(false);
	AddGameObject(floor);
	Drawable<> *screen = new Drawable<>({ "@screen.lua:Mesh", "@screen.dds", "@screen.lua:Material" });
	screen->SetCastShadow(false);
	AddGameObject(screen);
	Drawable<> *platform = new Drawable<>({ "@platform.lua:Mesh", "@platform.png", "@platform.lua:Material" });
	platform->SetCastShadow(false);
	platform->SetReceiveShadow(false);
	AddGameObject(platform);
	EPuck *epuck = new EPuck(Quaternion(0, 1, 0, 1), Vector3(-50, 0, 0));
	epuck->AddScript(new PythonScript("kbdControl"), false);
	AddGameObject(epuck);
	__lights.push_back(Light());
	__lights[0].SetDirectionalLight(Color(0xffffffff), Color(0xe0e0e0ff), Color(0x484848ff), Vector3(0.5f, -0.5f, 0.8f), Vector3(0, 0, 0), 76);
	GUIText *cameraInfo = new GUIText("Camera: Pos = (0, 0, 0) Dir = (0, 0, 0)", FontSheet::FontStyleRegular, Color(0, 0, 0), 3, GUITransform(1, 0, 1, 0, -7, 5, 0.35f, 0.35f, 0, 0xC));
	cameraInfo->AddScript(new PythonScript("cameraInfo"), false);
	AddGUIObject(cameraInfo);
	GUIText *fpsInfo = new GUIText("Logic FPS = 0\nRender FPS = 0", FontSheet::FontStyleRegular, Color(0, 0, 0), 3, GUITransform(0, 0, 0, 0, 7, 5, 0.35f, 0.35f, 0, 0xC));
	fpsInfo->AddScript(new PythonScript("fpsInfo"), false);
	AddGUIObject(fpsInfo);
	GUICameraView *overLookMap = new GUICameraView(1, GUITransform(1, 1, 1, 1, 0, 0, 400, 225, 0, 0xF));
	overLookMap->SetCameraSensor(overLookCamera, false);
	AddGUIObject(overLookMap);
	GUICameraView *robotViewColorMap = new GUICameraView(1, GUITransform(0, 1, 0, 1, 0, -212, 280, 210, 0, 0xF));
	robotViewColorMap->SetCameraSensor(epuck->GetCameraSensor(), false);
	AddGUIObject(robotViewColorMap);
	GUICameraView *robotViewDepthMap = new GUICameraView(1, GUITransform(0, 1, 0, 1, 0, 0, 280, 210, 0, 0xF));
	robotViewDepthMap->SetCameraSensor(epuck->GetCameraSensor(), true);
	AddGUIObject(robotViewDepthMap);
	EPuck *epuck2 = new EPuck(Quaternion(0, 1, 0, -1), Vector3(50, 0, 0));
	AddGameObject(epuck2);
	EPuck *epuck3 = new EPuck(Quaternion(0 ,0, 0, 1), Vector3(0, 0, -30));
	AddGameObject(epuck3);
	EPuck *epuck4 = new EPuck(Quaternion(0, 1, 0, 0), Vector3(0, 0, 30));
	AddGameObject(epuck4);
	EPuck *epuck5 = new EPuck(Quaternion(0, 0, 0, 1), Vector3(0, 0, 0));
	AddGameObject(epuck5);
	GUIImage *leftBottomPanel = new GUIImage("@brown.png", 0, GUITransform(0, 1, 0, 1, 0, 0, 282, 424, 0, 0xF));
	AddGUIObject(leftBottomPanel);
	GUIImage *rightBottomPanel = new GUIImage("@brown.png", 0, GUITransform(1, 1, 1, 1, 0, 0, 402, 227, 0, 0xF));
	AddGUIObject(rightBottomPanel);
	AddGameObject(new Wall("@yellow.png", -10.0f, -10.0f, -10.0f, -25.0f));
	AddGameObject(new Wall("@yellow.png", -10.0f, -10.0f, -50.0f, -10.0f));
	AddGameObject(new Wall("@blue.png", -10.0f, 10.0f, -10.0f, 25.0f));
	AddGameObject(new Wall("@blue.png", -10.0f, 10.0f, -50.0f, 10.0f));
	AddGameObject(new Wall("@blue.png", 10.0f, -10.0f, 10.0f, -25.0f));
	AddGameObject(new Wall("@blue.png", 10.0f, -10.0f, 50.0f, -10.0f));
	AddGameObject(new Wall("@yellow.png", 10.0f, 10.0f, 10.0f, 25.0f));
	AddGameObject(new Wall("@yellow.png", 10.0f, 10.0f, 50.0f, 10.0f));
	state = ReadyToUpdate;
}

void RobotSimulatorScene::PreRender(void *const param)
{
	cameraSensorList.clear();
	Scene::PreRender(&cameraSensorList);
}

void RobotSimulatorScene::Render(void *const param)
{
	assert(state == ReadyToRender);
	CheckStarted();
	assert(__mainCamera != NULL);
	RenderShadowMap();
	RenderScene();
	RenderManager *renderMgr = CoolEngineGame::Instance()->GetRenderManager();
	for (size_t i = 0; i < cameraSensorList.size(); ++i)
	{
		cameraSensorList[i]->GetDrawableTexture()->Reset();
		int oldMainLightIndex = __mainLightIndex;
		if (!cameraSensorList[i]->isRenderShadow())
		{
			__mainLightIndex = -1;
		}
		Effect *effect = renderMgr->GetEffectAt(0);
		Vector3 cameraPos(cameraSensorList[i]->__GetWorldPosition());
		effect->SetMatrix(cameraSensorList[i]->__GetViewProjMatrix(), 2);
		effect->SetRawValue(&cameraPos, sizeof(Vector3), 2);
		for (size_t i = 0; i < __renderList.size(); ++i)
		{
			__renderList[i]->Render(1, param);
		}
		__mainLightIndex = oldMainLightIndex;
	}
	state = ReadyToDraw;
}

void RobotSimulatorScene::Draw(void *const param)
{
	assert(state == ReadyToDraw);
	CheckStarted();
	RenderManager *renderMgr = CoolEngineGame::Instance()->GetRenderManager();
	renderMgr->UseTarget();
	std::vector<std::pair<Effect*, unsigned>> shaders = { { renderMgr->GetEffectAt(1), 1}, { renderMgr->GetEffectAt(4), 1 } };
	CoolEngineGame::Instance()->GetGUIManager()->SetNDCMatrixToShaders(shaders);
	for (std::list<GUIObject*>::iterator iter = __guiObjectList.begin(); iter != __guiObjectList.end(); ++iter)
	{
		(*iter)->Draw(param);
	}
	state = ReadyToPostRender;
}

RobotSimulator::RobotSimulator(HINSTANCE hInstance) : CoolEngineGame(hInstance)
{
	mainWndCaption = L"OSLab Robot Simulator";
}

bool RobotSimulator::InitRobotSimulator()
{
	if (!CoolEngineGame::Init("RobotSimulator/"))
	{
		return false;
	}

	GetRenderManager()->AddEffect(new DepthMapEffect(GetDevice()));

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;
	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_BLEND_FACTOR;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_BLEND_FACTOR;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	GetRenderManager()->AddBlendState(&transparentDesc);

	// Init Scene
	scene = new RobotSimulatorScene();
	scene->Start();

	return true;
}