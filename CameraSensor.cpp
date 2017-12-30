#include "CameraSensor.h"

using namespace CE;

CameraSensor::CameraSensor(const unsigned texWidth, const unsigned texHeight, const Transform &transform, Script **const scr, const bool isPspt, const float nr, const float fr, const float fOrW, const float depthFar)
	: drawableTexture(NULL), Camera(transform, scr, isPspt, nr, fr, fOrW, isPspt ? static_cast<float>(texWidth) / texHeight : texHeight*fOrW / texWidth), renderShadow(true), activated(true), depthSensorFar(depthFar)
{
	drawableTexture = new DrawableTexture(texWidth, texHeight);
	assert(near_ < depthSensorFar && depthSensorFar <= far_);
}

CameraSensor::~CameraSensor()
{
	if (drawableTexture)
	{
		delete drawableTexture;
	}
}

void CameraSensor::PreRender(std::vector<GameObject*> &__renderList, void *const param)
{
	Camera::PreRender(__renderList, param);
	if (param != NULL && activated)
	{
		reinterpret_cast<std::vector<CameraSensor*>*>(param)->push_back(this);
	}
}