#pragma once

#include "Application/Core.h"
#include "MeshComponent.h"
#include "Skeleton.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(const std::string &name = "SkeletalMeshComponent", const std::vector<SkeletalVertex> &vertices = {}, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault());
	virtual ~SkeletalMeshComponent() = default;

public:
	virtual const void *GetVertices() const override;
	virtual size_t GetVerticesLength() const override;
	virtual VertexType GetVertexType() const override;

public:
	Ref<Skeleton> skeleton; // TODO: unique ptr here
	std::vector<SkeletalVertex> vertices;
};
