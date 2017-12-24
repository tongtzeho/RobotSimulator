#include "Robot.h"
#include "RobotSimulator.h"
#include "../Engine/Render/WICTextureLoader.h"

using namespace CE;

RobotSimulatorScene::RobotSimulatorScene() : Scene(NULL, "@background.dds") {}

void RobotSimulatorScene::Start(void *const param)
{
	Scene::Start();
	Camera *overLookCamera = new Camera(Transform(Vector3(1, 1, 1), Quaternion(0.7071f, 0, 0, 0.707101f), Vector3(0, 890, 0)), NULL, Vector3(0, 0, 1), Vector3(0, 1, 0), false, 1.02f, 10.0f, 3000.0f);
	AddGameObject(overLookCamera);
	Camera *freeCamera = new Camera(Transform(Vector3(1, 1, 1), Quaternion(0.5f, 0, 0, 0.866025403784f), Vector3(0, 500, -360)), NULL, Vector3(0, 0, 1), Vector3(0, 1, 0), true, 0.333333333f*Pi, 1.0f, 100000.0f);
	freeCamera->AddScript(new PythonScript("freeCamera"), false);
	AddGameObject(freeCamera);
	char pyScriptParam[64] = { 0 };
	sprintf(pyScriptParam, "%lld %lld", reinterpret_cast<long long>(overLookCamera), reinterpret_cast<long long>(freeCamera));
	AddScript(new PythonScript("switchCamera", pyScriptParam));
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
	EPuck *epuck = new EPuck(Quaternion(), Vector3(500, 0, 0));
	epuck->AddScript(new PythonScript("kbdControl"), false);
	AddGameObject(epuck);
	lights.push_back(Light());
	lights[0].SetDirectionalLight(Color(0xffffffff), Color(0xc0c0c0ff), Color(0xd8d8d8ff), Vector3(0.5f, -0.5f, 0.8f), Vector3(0, 0, 0), 800);
	GUIText *cameraInfo = new GUIText("Camera: Pos = (0, 0, 0) Dir = (0, 0, 0)", FontSheet::FontStyleRegular, 0xff000000, 3, GUITransform(0, 0, 0, 0, 8, 5, 0.35f, 0.35f, 0, 0xC));
	cameraInfo->AddScript(new PythonScript("cameraInfo"), false);
	AddGUIObject(cameraInfo);
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

	// Init Scene
	scene = new RobotSimulatorScene();
	scene->Start();

	return true;
}