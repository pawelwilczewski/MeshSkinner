#include "pch.h"
#include "StaticMeshComponent.h"

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
