#pragma once

#include "Application/Core.h"
#include "Core/Entity/Transform.h"

class Bone : public Entity
{
public:
	Bone();

public:
	glm::mat4 inverseBindMatrix = glm::mat4(1.f);
};

class Skeleton
{
public:
	Skeleton();

	// TODO: [] operator which gives the bone by string name

public:
	std::vector<Ref<Bone>> bones;
	uint16_t root = -1;
};
