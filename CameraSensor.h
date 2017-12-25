#pragma once

#include "../Engine/Scene/Camera.h"
#include "../Engine/Render/DrawableTexture.h"

class CameraSensor : public CE::Camera
{
	CE::DrawableTexture *drawableTexture;
	bool renderShadow;
	bool activated;
public:
	CameraSensor(const unsigned texWidth, const unsigned texHeight, const CE::Transform &transform = CE::Transform(), CE::Script **const scr = NULL, const bool isPspt = true, const float nr = 1, const float fr = 100, const float fOrW = 0.25f*CE::Pi); // ͸��������Ŀ�߱Ȼ�����������ĸ߿��Ը���������ֵ�ó�
	virtual ~CameraSensor();
	virtual void PreRender(void *const param = NULL);
	inline bool isRenderShadow() const { return renderShadow; }
	inline void SetRenderShadow(const bool renderShadow) { this->renderShadow = renderShadow; }
	inline CE::DrawableTexture *GetDrawableTexture() const { return drawableTexture; }
};