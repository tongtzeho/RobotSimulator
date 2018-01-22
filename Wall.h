#pragma once

#include "../Engine/Scene/Renderable.h"

class Wall : public CE::Renderable<>
{
public:
	Wall(const std::string &texResource, const float x1, const float z1, const float x2, const float z2);
};