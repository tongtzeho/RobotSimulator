#pragma once

#include "../Engine/Scene/Camera.h"
#include "../Engine/Render/DrawableTexture.h"

class CameraSensor : public CE::Camera
{
	CE::DrawableTexture *drawableTexture;
	float depthSensorFar; // 深度摄像头实际上的最大深度
	bool renderShadow;
	bool activated;
public:
	CameraSensor(const unsigned texWidth, const unsigned texHeight, const CE::Transform &transform = CE::Transform(), CE::Script **const scr = NULL, const bool isPspt = true, const float nr = 1, const float fr = 100, const float fOrW = 0.25f*CE::Pi, const float depthFar = 50); // 透视摄像机的宽高比或正交摄像机的高可以根据其他数值得出
	virtual ~CameraSensor();
	virtual void PreRender(std::vector<GameObject*> &__renderList, void *const param = NULL);
	inline bool isRenderShadow() const { return renderShadow; }
	inline void SetRenderShadow(const bool renderShadow) { this->renderShadow = renderShadow; }
	inline float GetDepthSensorFar() const { return depthSensorFar; }
	inline CE::DrawableTexture *GetDrawableTexture() const { return drawableTexture; }
};