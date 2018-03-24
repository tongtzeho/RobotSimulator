#pragma once

#include "../Engine/CoolEngine.h"
#include "IActionController.h"

class RobotSimulator : public CE::CoolEngine
{
private:
	std::unordered_map<unsigned, IActionController*(*)(CE::IComponent*, const void*)> instancingActionControllerFuncTable;
	virtual void InitPython(const char *rootDir = nullptr);
public:
	RobotSimulator(HINSTANCE hInstance);
	virtual ~RobotSimulator();
	bool RegisterActionController(const std::string actionControllerName, IActionController*(*instancingActionControllerFunc)(CE::IComponent*, const void*));
	virtual bool Init(const char *rootDir = nullptr);
	IActionController* InstancingActionController(const char *actionControllerName, CE::IComponent *const comp, const void *actionControllerParam);
};