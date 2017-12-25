#include "CameraSensor.h"

using namespace CE;

CameraSensor::CameraSensor(const unsigned texWidth, const unsigned texHeight, const Transform &transform, Script **const scr, const bool isPspt, const float nr, const float fr, const float fOrW)
	: drawableTexture(NULL), Camera(transform, scr, isPspt, nr, fr, fOrW, isPspt ? static_cast<float>(texWidth) / texHeight : texHeight*fOrW / texWidth), renderShadow(true), activated(true)
{
	drawableTexture = new DrawableTexture(texWidth, texHeight);
}

CameraSensor::~CameraSensor()
{
	if (drawableTexture)
	{
		delete drawableTexture;
	}
}

void CameraSensor::PreRender(void *const param)
{
	Camera::PreRender(param);
	if (param != NULL && activated)
	{
		reinterpret_cast<std::vector<CameraSensor*>*>(param)->push_back(this);
	}
}