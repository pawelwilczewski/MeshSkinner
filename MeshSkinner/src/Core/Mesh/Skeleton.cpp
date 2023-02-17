#include "pch.h"
#include "Skeleton.h"

Bone::Bone()
{

}

Bone::Bone(const std::string &name, uint16_t parentIndex, const Transform &localTransform, const glm::mat4 &inverseBindMatrix) : name(name), parentIndex(parentIndex), localTransform(localTransform), inverseBindMatrix(inverseBindMatrix)
{

}

Skeleton::Skeleton()
{
}
