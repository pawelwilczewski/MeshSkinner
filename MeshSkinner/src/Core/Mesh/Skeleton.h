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
	friend class MeshLibrary;

public:
	Skeleton();

public:
	const Ref<Bone> &operator[](const std::string &boneName) const;
	const Ref<Bone> &GetRootBone() const;

	const std::vector<Ref<Bone>> &GetBones() const;

private:
	std::vector<Ref<Bone>> bones;
	uint16_t root = -1;
};
