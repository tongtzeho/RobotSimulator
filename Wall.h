#pragma once

#include "../Engine/Scene/Drawable.h"

class Wall : public CE::Drawable<>
{
public:
	Wall(const std::string &texResource, const float x1, const float z1, const float x2, const float z2);
};