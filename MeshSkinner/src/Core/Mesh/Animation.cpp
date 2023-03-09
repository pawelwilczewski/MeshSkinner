#include "pch.h"
#include "Animation.h"

Animation::Animation(const std::string &name, bool loop) : name(name), loop(loop)
{
}

glm::vec3 Animation::EvaluateTranslation(const std::string &boneName, float time) const
{
	// TODO: switch on interpolation mode and use this as a reference: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#appendix-c-interpolation

	auto &track = tracks.at(boneName);
	auto &positions = track.translationKeyframes;

	if (positions.size() > 1 && loop)
		time = fmod(time, positions.back().time);

	for (size_t i = 1; i < positions.size(); i++)
	{
		if (time <= positions[i].time)
		{
			auto alpha = (time - positions[i - 1].time) / (positions[i].time - positions[i - 1].time);

			return glm::mix(positions[i - 1].value, positions[i].value, alpha);
		}
	}

	return glm::vec3(0.f);
}

glm::quat Animation::EvaluateRotation(const std::string &boneName, float time) const
{
	// TODO: switch on interpolation mode and use this as a reference: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#appendix-c-interpolation

	auto &track = tracks.at(boneName);
	auto &rotations = track.rotationKeyframes;

	if (rotations.size() > 1 && loop)
		time = fmod(time, rotations.back().time);

	for (size_t i = 1; i < rotations.size(); i++)
	{
		if (time <= rotations[i].time)
		{
			auto alpha = (time - rotations[i - 1].time) / (rotations[i].time - rotations[i - 1].time);

			return glm::lerp(rotations[i - 1].value, rotations[i].value, alpha);
		}
	}

	return glm::quat();
}

glm::vec3 Animation::EvaluateScale(const std::string &boneName, float time) const
{
	// TODO: switch on interpolation mode and use this as a reference: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#appendix-c-interpolation

	auto &track = tracks.at(boneName);
	auto &scales = track.scaleKeyframes;

	if (scales.size() > 1 && loop)
		time = fmod(time, scales.back().time);

	for (size_t i = 1; i < scales.size(); i++)
	{
		if (time <= scales[i].time)
		{
			auto alpha = (time - scales[i - 1].time) / (scales[i].time - scales[i - 1].time);

			return glm::mix(scales[i - 1].value, scales[i].value, alpha);
		}
	}

	return glm::vec3(1.f);
}

glm::mat4 Animation::Evaluate(const std::string &boneName, float time) const
{
	auto result = glm::translate(glm::mat4(1.f), EvaluateTranslation(boneName, time));
	result *= glm::toMat4(EvaluateRotation(boneName, time));
	result = glm::scale(result, EvaluateScale(boneName, time));

	return result;
}

float Animation::GetTimeUsedForEvaluation(float timeSeconds) const
{
	if (loop)
		return fmod(timeSeconds, duration);
	else
		return glm::clamp(timeSeconds, 0.f, duration);
}

float Animation::GetDuration() const
{
	return duration;
}
