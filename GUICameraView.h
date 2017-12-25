#pragma once

#include "../Engine/GUI/GUIImage.h"

class GUICameraView : public CE::GUIImage
{
private:
	bool isDepthMap;
public:
	GUICameraView(const int depth = 0, const CE::GUITransform &guiTransform = CE::GUITransform(), CE::Script **const scr = NULL) : GUIImage(depth, guiTransform, scr), isDepthMap(false) {}
	inline void SetTexture(CE::Texture *const tex, const bool isDepthMap) { CE::GUIImage::SetTexture(tex); this->isDepthMap = isDepthMap; }
	virtual void Draw(void *const param = NULL);
};