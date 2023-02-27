#include "pch.h"
#include "Animation.h"

Animation::Animation() : keyframes(std::unordered_map<const char *, std::vector<Keyframe>>())
{
}

glm::mat4 Animation::Evaluate(const char *boneName, float time)
{
	auto &frames = keyframes.at(boneName);

	assert(frames.size() > 0);

	for (size_t i = 1; i < frames.size(); i++)
	{
		if (time <= frames[i].time)
		{
			auto alpha = (time - frames[i - 1].time) / (frames[i].time - frames[i - 1].time);
			auto position = glm::mix(frames[i - 1].transform.GetPosition(), frames[i].transform.GetPosition(), alpha);
			auto rotation = glm::lerp(glm::quat(glm::radians(frames[i - 1].transform.GetRotation())), glm::quat(glm::radians(frames[i].transform.GetRotation())), alpha);
			auto scale = glm::mix(frames[i - 1].transform.GetScale(), frames[i].transform.GetScale(), alpha);

			auto result = glm::translate(glm::mat4(1.f), position);
			result *= glm::toMat4(rotation);
			result = glm::scale(result, scale);

			return result;
		}
	}

	return frames[frames.size() - 1].transform.GetMatrix();
}
