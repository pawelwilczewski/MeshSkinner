#include "pch.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(const std::string &name, const std::vector<uint32_t> &indices, Ref<Material> material) : EntityComponent(name), indices(indices), material(material)
{

}

BufferLayout MeshComponent::GetVertexBufferLayout() const
{
    switch (GetVertexType())
    {
    case Static:
        return StaticVertex::layout;
    case Skeletal:
        return SkeletalVertex::layout;
    default:
        assert(false);
        return BufferLayout({});
    }
}
