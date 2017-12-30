#pragma once

#include "../Engine/Scene/Camera.h"
#include "../Engine/Render/DrawableTexture.h"

class CameraSensor : public CE::Camera
{
	CE::DrawableTexture *drawableTexture;
	float depthSensorFar; // �������ͷʵ���ϵ�������
	bool renderShadow;
	bool activated;
public:
	CameraSensor(const unsigned texWidth, const unsigned texHeight, const CE::Transform &transform = CE::Transform(), CE::Script **const scr = NULL, const bool isPspt = true, const float nr = 1, const float fr = 100, const float fOrW = 0.25f*CE::Pi, const float depthFar = 50); // ͸��������Ŀ�߱Ȼ�����������ĸ߿��Ը���������ֵ�ó�
	virtual ~CameraSensor();
	virtual void PreRender(std::vector<GameObject*> &__renderList, void *const param = NULL);
	inline bool isRenderShadow() const { return renderShadow; }
	inline void SetRenderShadow(const bool renderShadow) { this->renderShadow = renderShadow; }
	inline float GetDepthSensorFar() const { return depthSensorFar; }
	inline CE::DrawableTexture *GetDrawableTexture() const { return drawableTexture; }
};