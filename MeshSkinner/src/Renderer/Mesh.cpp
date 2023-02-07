#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<uint32_t> indices, Ref<Material> material) : indices(indices), material(material)
{

}

StaticMesh::StaticMesh(const std::vector<StaticVertex> vertices, const std::vector<uint32_t> indices, Ref<Material> material) : Mesh(indices, material), vertices(vertices)
{

}

SkeletalMesh::SkeletalMesh(const std::vector<SkeletalVertex> vertices, const std::vector<uint32_t> indices, Ref<Material> material) : Mesh(indices, material), vertices(vertices)
{

}
