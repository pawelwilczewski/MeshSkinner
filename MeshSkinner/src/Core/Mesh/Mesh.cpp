#include "pch.h"
#include "Mesh.h"

MeshComponent::MeshComponent(const std::string &name, const std::vector<uint32_t> &indices, Ref<Material> material) : EntityComponent(name), indices(indices), material(material)
{

}

const BufferLayout &MeshComponent::GetVertexBufferLayout() const
{
    switch (GetVertexType())
    {
    case Static:
        return StaticVertex::layout;
    case Skeletal:
        return SkeletalVertex::layout;
    default:
        assert(false);
    }
}

StaticMeshComponent::StaticMeshComponent(const std::string &name, const std::vector<StaticVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material) : MeshComponent(name, indices, material), vertices(vertices)
{

}

const void *StaticMeshComponent::GetVertices() const
{
    return vertices.data();
}

MeshComponent::VertexType StaticMeshComponent::GetVertexType() const
{
    return Static;
}

size_t StaticMeshComponent::GetVerticesLength() const
{
    return vertices.size();
}

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
