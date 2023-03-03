#pragma once

class Stroke
{
public:
	enum class Type { PixelDistance = 0, WorldDistance, EachFrame, FixedDeltaTime };

public:
	Stroke(Type type = Type::PixelDistance, float pixelDistance = 10.f, float worldDistance = 1.f, float fixedIntervalTime = 0.1f);
	virtual ~Stroke() = default;

public:
	Type type;
	float pixelDistance;
	float worldDistance;
	float fixedIntervalTime;
};
