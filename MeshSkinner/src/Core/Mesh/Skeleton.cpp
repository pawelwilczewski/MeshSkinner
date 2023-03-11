#include "pch.h"
#include "Skeleton.h"

Bone::Bone() : Entity()
{

}

Skeleton::Skeleton()
{
}

Bone *Skeleton::operator[](const char *boneName) const
{
    return GetBoneByName(boneName);
}

Bone *Skeleton::GetBoneByName(const char *boneName) const
{
    for (const auto &bone : bones)
        if (bone->name == boneName)
            return bone;

    return nullptr;
}

Bone *Skeleton::GetRootBone() const
{
    return bones[root];
}

const std::vector<Bone *> &Skeleton::GetBones() const
{
    return bones;
}

BoneGPU::BoneGPU(Bone &bone, const glm::mat4 &inverseRootWorldMatrix) : modelMatrix(bone.GetWorldMatrix() * inverseRootWorldMatrix), inverseBindMatrix(bone.inverseBindMatrix)
{

}
