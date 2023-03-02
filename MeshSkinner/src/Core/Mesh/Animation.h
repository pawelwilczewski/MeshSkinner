#pragma once

#include "Application/Core.h"
#include "Keyframe.h"

class Animation
{
	friend class MeshLibrary;

public:
	Animation(bool loop = true);

public:
	glm::mat4 Evaluate(const char *boneName, float time);

private:
	std::unordered_map<const char *, std::vector<Keyframe>> keyframes;

public:
	// TODO: interpolation mode
	bool loop;
};
