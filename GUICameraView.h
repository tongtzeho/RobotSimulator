#pragma once

#include "../Engine/GUI/GUIImage.h"
#include "CameraSensor.h"

class GUICameraView : public CE::GUIImage
{
private:
	CameraSensor *cameraSensor;
	float depthSensorFar;
	float near_;
	float far_;
	bool isDepthMap;
	bool isPerspective;
public:
	GUICameraView(const int depth = 0, const CE::GUITransform &guiTransform = CE::GUITransform(), CE::Script **const scr = nullptr) : GUIImage(depth, guiTransform, scr), cameraSensor(nullptr), isDepthMap(false) {}
	void SetCameraSensor(CameraSensor *const cameraSensor, const bool isDepthMap = false);
	virtual void Draw(void *const param = nullptr);
};