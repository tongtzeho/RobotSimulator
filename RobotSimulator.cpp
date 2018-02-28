#include "RGBSensor.h"
#include "DepthSensor.h"
#include "EpuckRenderer.h"
#include "RobotSimulatorScene.h"
#include "RobotSimulator.h"
#include "PyRobotSimulator.h"

using namespace CE;

RobotSimulator::RobotSimulator(HINSTANCE hInstance) : CoolEngine(hInstance)
{
	mainWndCaption = L"PKU OSLab Robot Simulator - Cool Engine";
}

void RobotSimulator::InitPython(const char *rootDir)
{
	CoolEngine::InitPython(rootDir);
	PyRobotSimulator::Initialize();
}

bool RobotSimulator::Init(const char *rootDir)
{
	if (!CoolEngine::Init(rootDir))
	{
		return false;
	}

	/* ����Զ���Component */
	Entity::RegisterComponent<RGBSensor>("RGBSensor");
	Entity::RegisterComponent<DepthSensor>("DepthSensor");

	ID3D11Device *const d3d11Device = GetDevice();
	RenderManager *const renderMgr = GetRenderManager();

	/* ����Զ���Shader */
	renderMgr->RegisterBinaryEffect("$Epuck");

	/* ����Զ���Renderer */
	renderMgr->RegisterRenderer("EpuckRenderer", EpuckRenderer::Instancing);

	/* ���������س��� */
	scene = new RobotSimulatorScene();
	scene->Init("@simscene.lua:Scene");

	return true;
}

// �������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef PY_CONSOLE
	AllocConsole(); // ��������̨
	::freopen("CONOUT$", "w", stdout); // Configuration Properties -> C/C++ -> Preprocessor  add _CRT_SECURE_NO_WARNINGS
	::freopen("CONOUT$", "w", stderr);
	::freopen("CONIN$", "r", stdin);
#endif // PY_CONSOLE

	RobotSimulator robotSimulator(hInstance);

	if (!robotSimulator.Init("RobotSimulator/")) // ����Ŀ¼RobotSimulator/
	{
		return 0;
	}

	return robotSimulator.Run();
}
