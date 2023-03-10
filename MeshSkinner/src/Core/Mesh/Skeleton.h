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

struct BoneGPU
{
	BoneGPU(Bone &bone, const glm::mat4 &inverseRootWorldMatrix);

	glm::mat4 modelMatrix;
	glm::mat4 inverseBindMatrix;
};

class Skeleton
{
	friend class MeshLibrary;

public:
	Skeleton();

public:
	const Bone *operator[](const char *boneName) const;
	const Bone *GetBoneByName(const char *boneName) const;
	const Ref<Bone> &GetRootBone() const;

	const std::vector<Ref<Bone>> &GetBones() const;

private:
	std::vector<Ref<Bone>> bones; // TODO: unique ptrs here
	uint16_t root = -1;
};
