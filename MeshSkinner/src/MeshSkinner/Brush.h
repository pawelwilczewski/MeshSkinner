#pragma once

#include "Application/Core.h"

class Brush
{
public:
	enum class BlendMode { Linear = 0, Add, Multiply, Gaussian, Mix };

public:
	Brush(BlendMode blendMode = BlendMode::Add, float weight = 1.f, float radius = 10.f, float falloff = 1.f, float strength = 1.f);
	virtual ~Brush() = default;

public:
	bool DisplayUI(const char *windowName);

public:
	float Blend(float oldWeight, float newWeight);

public:
	BlendMode blendMode;
	float weight;
	float radius;
	float falloff;
	float strength;
	// TODO: falloff radius
};
