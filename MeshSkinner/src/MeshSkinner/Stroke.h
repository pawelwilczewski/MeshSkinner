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
	Stroke(const std::string &toolWindowName, const std::function<void(StrokeQueryInfo &)> &queryFunction, Type type = Type::WorldDistance, float pixelDistance = 10.f, float worldDistance = 1.f, float fixedIntervalTime = 0.1f);
	virtual ~Stroke();

protected:
	virtual void OnUpdateUI() override;

public:
	void SetType(Type newType);

public:
	void OnStrokeEmplaceSubscribe(const CallbackRef<StrokeQueryInfo> &callback);
	void OnStrokeEmplaceUnsubscribe(const CallbackRef<StrokeQueryInfo> &callback);

private:
	void OnStrokeUpdate();
	void OnMouseButtonUp(int button);
	void UpdateSubscribe() const;
	void UpdateUnsubscribe() const;

public:
	float pixelDistance;
	float worldDistance;
	float fixedIntervalTime;

private:
	Type type;

private:
	glm::vec2 lastViewportPosition = glm::vec2(0.f);
	glm::vec3 lastWorldPosition = glm::vec3(0.f);

	bool initialContact = true;

private:
	std::function<void(StrokeQueryInfo &)> queryFunction;

private:
	Event<StrokeQueryInfo> onStrokeEmplace;

	CallbackNoArgRef onStrokeUpdateCallback;
	// required because onStrokeUpdate is NoArg so we just call it with no arguments via this
	CallbackRef<glm::vec2> onStrokeUpdateVec2CallbackWrapper;

	// restart the stroke info
	CallbackRef<int> onMouseButtonUpCallback;
};
