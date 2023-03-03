#pragma once

#include "Application/Core.h"

class Brush
{
public:
	Brush(int blendMode = static_cast<int>(MathUtils::BlendMode::Add), float weight = 1.f, float radius = 10.f, float falloff = 1.f, float strength = 1.f);
	virtual ~Brush() = default;

public:
	bool DisplayUI(const char *windowName);

public:
	int blendMode;
	float weight;
	float radius;
	float falloff;
	float strength;
	// TODO: falloff radius
};
