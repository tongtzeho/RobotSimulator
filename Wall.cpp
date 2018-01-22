#include "Wall.h"

using namespace CE;

Wall::Wall(const std::string &texResource, const float x1, const float z1, const float x2, const float z2)
	: Renderable<>({ "@wall.lua:Mesh", texResource, "@wall.lua:Material" })
{
	SetScalingX(sqrt((x1 - x2)*(x1 - x2) + (z1 - z2)*(z1 - z2)) / 2.0f);
	SetEulerRadian(Vector3(0, -atan2(z2 - z1, x2 - x1), 0));
	SetTranslation(Vector3((x1 + x2)*0.5f, 0, (z1 + z2)*0.5f));
}