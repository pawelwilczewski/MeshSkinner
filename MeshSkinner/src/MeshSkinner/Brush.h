#pragma once

#include "Application/Core.h"
#include "Tool.h"

class Brush : Tool
{
public:
	// NOTE: upon updating any of these, ALSO update the strings array at the top of Brush.cpp
	enum class BlendMode { Mix = 0, Add, Subtract, Lighten, Darken, ColorDodge, Difference, Screen, HardLight, Overlay, SoftLight, Exclusion };

public:
	Brush(const std::string &toolWindowName, BlendMode blendMode = BlendMode::Mix, float weight = 1.f, float radius = 10.f, float falloff = 0.5f, float strength = 1.f);
	virtual ~Brush() = default;

protected:
	virtual void OnUpdateUI() override;

public:
	float Blend(float oldWeight, float distance);

public:
	BlendMode blendMode;
	float weight;
	float radius;
	float falloff;
	float strength;
	// TODO: falloff radius
};
