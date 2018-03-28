#include "RGBSensor.h"
#include "DepthSensor.h"
#include "DistanceSensor.h"
#include "ProximitySensor.h"
#include "IMUSensor.h"
#include "EpuckRenderer.h"
#include "LODRenderer.h"
#include "EpuckActionController.h"
#include "RobotSimulatorScene.h"
#include "RobotSimulator.h"
#include "PyRobotSimulator.h"
#include "SocketServerConsole.h"

using namespace CE;

RobotSimulator::RobotSimulator(HINSTANCE hInstance) : CoolEngine(hInstance)
{
	mainWndCaption = L"PKU OSLab Robot Simulator - Cool Engine";
}

RobotSimulator::~RobotSimulator()
{
	SocketServerConsole::Destroy();
}

bool RobotSimulator::RegisterActionController(const std::string actionControllerName, IActionController*(*instancingActionControllerFunc)(IComponent*, const void*))
{
	unsigned actionControllerNameHash = String::HashString(actionControllerName);
	std::unordered_map<unsigned, IActionController*(*)(IComponent*, const void*)>::const_iterator iter = instancingActionControllerFuncTable.find(actionControllerNameHash);
	if (iter == instancingActionControllerFuncTable.end())
	{
		instancingActionControllerFuncTable[actionControllerNameHash] = instancingActionControllerFunc;
		return true;
	}
	else
	{
		return false;
	}
}

IActionController* RobotSimulator::InstancingActionController(const char *actionControllerName, IComponent *const comp, const void *actionControllerParam)
{
	assert(actionControllerName != nullptr && comp != nullptr);
	std::unordered_map<unsigned, IActionController*(*)(IComponent*, const void*)>::const_iterator iter = instancingActionControllerFuncTable.find(String::HashString(actionControllerName));
	if (iter == instancingActionControllerFuncTable.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second(comp, actionControllerParam);
	}
}

void RobotSimulator::InitPython(const char *rootDir)
{
	CoolEngine::InitPython(rootDir);
	PyRobotSimulator::Initialize();
	SocketServerConsole::Initialize();
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
	Entity::RegisterComponent<DistanceSensor>("DistanceSensor");
	Entity::RegisterComponent<ProximitySensor>("ProximitySensor");
	Entity::RegisterComponent<IMUSensor>("IMUSensor");
	Entity::RegisterComponent<IActionController>("ActionController");
	Entity::RegisterComponent<Communicator>("Communicator");

	RenderManager *const renderMgr = GetRenderManager();

	/* ����Զ���Shader */
	renderMgr->RegisterBinaryEffect("$Epuck");

	/* ����Զ���Renderer */
	renderMgr->RegisterRenderer("EpuckRenderer", EpuckRenderer::Instancing);
	renderMgr->RegisterRenderer("LODRenderer", LODRenderer::Instancing);

	/* ��ӻ������ж������� */
	RegisterActionController("EpuckActionController", EpuckActionController::Instancing);

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
