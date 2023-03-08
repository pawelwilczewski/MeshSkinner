#pragma once

#include "Application/Core.h"
#include "Core/Entity/Transform.h"

template <typename T>
struct Keyframe
{
	Keyframe(float time, const T &value) : time(time), value(value) {}

	float time;
	T value;
};
