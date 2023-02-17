#pragma once

#include "Application/Core.h"
#include "Core/Entity/Transform.h"

class Bone
{
public:
	Bone();
	Bone(const std::string &name, uint16_t parentIndex, const Transform &localTransform, const glm::mat4& inverseBindMatrix);

public:
	std::string name;
	uint16_t parentIndex;
	Transform localTransform;
	glm::mat4 inverseBindMatrix;
};

class Skeleton
{
public:
	Skeleton();

	// TODO: [] operator which gives the bone by string name

public:
	std::vector<Bone> bones;
	uint16_t root;
};
