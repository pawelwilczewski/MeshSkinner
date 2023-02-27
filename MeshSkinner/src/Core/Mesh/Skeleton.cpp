#include "pch.h"
#include "Skeleton.h"

Bone::Bone() : Entity()
{

}

Skeleton::Skeleton()
{
}

const Ref<Bone> &Skeleton::operator[](const std::string &boneName) const
{
    for (const auto &bone : bones)
        if (bone->name == boneName)
            return bone;

    return nullptr;
}

const Ref<Bone> &Skeleton::GetRootBone() const
{
    return bones[root];
}

const std::vector<Ref<Bone>> &Skeleton::GetBones() const
{
    return bones;
}
