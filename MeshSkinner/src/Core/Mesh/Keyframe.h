#pragma once

#include "Application/Core.h"
#include "Core/Entity/Transform.h"

struct Keyframe
{
	Keyframe(float time = 0.f, const Transform &transform = Transform());

public:
	float time;
	Transform transform;
};
