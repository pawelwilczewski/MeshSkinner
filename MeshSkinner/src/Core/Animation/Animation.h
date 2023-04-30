#pragma once

#include "Keyframe.h"

enum class AnimationInterpolationMode { Linear = 0, Step, CubicSpline };

struct AnimationTrack
{
	AnimationInterpolationMode interpolation = AnimationInterpolationMode::Linear;
	std::vector<Keyframe<glm::vec3>> translationKeyframes;
	std::vector<Keyframe<glm::quat>> rotationKeyframes;
	std::vector<Keyframe<glm::vec3>> scaleKeyframes;
	std::vector<Keyframe<float>> weightsKeyframes;
};

class Animation
{
	friend class MeshLibrary;

public:
	Animation(const std::string &name, bool loop = true);

public:
	glm::vec3 EvaluateTranslation(const std::string &boneName, float time) const;
	glm::quat EvaluateRotation(const std::string &boneName, float time) const;
	glm::vec3 EvaluateScale(const std::string &boneName, float time) const;
	glm::mat4 Evaluate(const std::string &boneName, float time) const;

public:
	float GetTimeUsedForEvaluation(float timeSeconds) const;

	float GetDuration() const;

private:
	std::unordered_map<std::string, AnimationTrack> tracks;

public:
	// TODO: interpolation mode
	bool loop;
	std::string name;

private:
	float duration = 0.f;
};
