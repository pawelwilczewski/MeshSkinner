#pragma once

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
	BoneGPU(Bone &bone);
	glm::mat4 inverseBindMatrix;
};

class Skeleton
{
	friend class MeshLibrary;

public:
	Skeleton();

public:
	Bone *operator[](const char *boneName) const;
	Bone *GetBoneByName(const char *boneName) const;
	Bone *GetRootBone() const;

	const std::vector<Bone *> &GetBones() const;

private:
	std::vector<Bone *> bones;
	uint16_t root = -1;
};
