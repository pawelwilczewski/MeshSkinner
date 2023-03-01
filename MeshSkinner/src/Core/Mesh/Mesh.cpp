#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : EntityComponent(), indices(indices), material(material), isStatic(isStatic)
{

}

const BufferLayout &Mesh::GetVertexBufferLayout() const
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

StaticMesh::StaticMesh(const std::vector<StaticVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : Mesh(indices, material, isStatic), vertices(vertices)
{

}

const void *StaticMesh::GetVertices() const
{
    return vertices.data();
}

Mesh::VertexType StaticMesh::GetVertexType() const
{
    return Static;
}

SkeletalMesh::SkeletalMesh(const std::vector<SkeletalVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : Mesh(indices, material, isStatic), vertices(vertices)
{

}

const void *SkeletalMesh::GetVertices() const
{
    return vertices.data();
}

Mesh::VertexType SkeletalMesh::GetVertexType() const
{
    return Skeletal;
}
