#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : EntityComponent(), indices(indices), material(material), isStatic(isStatic)
{

}

StaticMesh::StaticMesh(const std::vector<StaticVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : Mesh(indices, material, isStatic), vertices(vertices)
{

}

SkeletalMesh::SkeletalMesh(const std::vector<SkeletalVertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material, bool isStatic) : Mesh(indices, material, isStatic), vertices(vertices)
{

}
