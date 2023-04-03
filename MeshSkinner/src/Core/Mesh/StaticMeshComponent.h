#pragma once

#include "MeshComponent.h"

class StaticMeshComponent : public MeshComponent
{
public:
	StaticMeshComponent(const std::string &name = "StaticMeshComponent", const std::vector<StaticVertex> &vertices = {}, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault());
	virtual ~StaticMeshComponent() = default;

public:
	virtual const void *GetVertices() const override;
	virtual size_t GetVerticesLength() const override;
	virtual VertexType GetVertexType() const override;

public:
	std::vector<StaticVertex> vertices;
};
