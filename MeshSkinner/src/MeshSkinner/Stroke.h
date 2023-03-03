#pragma once

#include "Application/Core.h"
#include "Tool.h"

struct StrokeQueryInfo
{
	StrokeQueryInfo(bool hitTarget = false, const glm::vec3 &worldPosition = glm::vec3(0.f), const glm::vec2 &viewportPosition = glm::vec2(0.f));

	bool hitTarget;
	glm::vec3 worldPosition;
	glm::vec2 viewportPosition;
};

class Stroke : public Tool
{
public:
	enum class Type { PixelDistance = 0, WorldDistance, EachFrame, FixedDeltaTime };

public:
	Stroke(const std::string &toolWindowName, Type type = Type::PixelDistance, float pixelDistance = 10.f, float worldDistance = 1.f, float fixedIntervalTime = 0.1f);
	virtual ~Stroke();

private:
	void OnStrokeUpdate();

protected:
	virtual void OnUpdateUI() override;

public:
	Type type;
	float pixelDistance;
	float worldDistance;
	float fixedIntervalTime;

private:
	Event<StrokeQueryInfo> onStrokeEmplace;

	CallbackNoArgRef onStrokeUpdateCallback;
};
