#pragma once

#include "Application/Core.h"
#include "Keyframe.h"

enum class AnimationInterpolationMode { Linear = 0, Step, CubicSpline };

struct AnimationTrack
{
	AnimationInterpolationMode interpolation;
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
	glm::vec3 EvaluateTranslation(const std::string &boneName, float time);
	glm::quat EvaluateRotation(const std::string &boneName, float time);
	glm::vec3 EvaluateScale(const std::string &boneName, float time);
	glm::mat4 Evaluate(const std::string &boneName, float time);

private:
	std::unordered_map<std::string, AnimationTrack> tracks;

public:
	// TODO: interpolation mode
	bool loop;
	std::string name;
};
