#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::string &name, const std::vector<uint32_t> &indices, Ref<Material> material) : EntityComponent(name), indices(indices), material(material)
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

StaticMesh::StaticMesh(const std::string &name, const std::vector<StaticVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material) : Mesh(name, indices, material), vertices(vertices)
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

size_t StaticMesh::GetVerticesLength() const
{
    return vertices.size();
}

SkeletalMesh::SkeletalMesh(const std::string &name, const std::vector<SkeletalVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material) : Mesh(name, indices, material), vertices(vertices)
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

size_t SkeletalMesh::GetVerticesLength() const
{
    return vertices.size();
}
