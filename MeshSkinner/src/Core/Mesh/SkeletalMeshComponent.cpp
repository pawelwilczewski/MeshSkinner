#include "pch.h"
#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent(const std::string &name, const std::vector<SkeletalVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material) : MeshComponent(name, indices, material), vertices(vertices)
{

}

const void *SkeletalMeshComponent::GetVertices() const
{
    return vertices.data();
}

MeshComponent::VertexType SkeletalMeshComponent::GetVertexType() const
{
    return Skeletal;
}

size_t SkeletalMeshComponent::GetVerticesLength() const
{
    return vertices.size();
}
