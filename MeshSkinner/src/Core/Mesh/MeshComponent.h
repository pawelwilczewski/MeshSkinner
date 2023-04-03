#pragma once

#include "Vertex.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Renderer/Material/MaterialLibrary.h"

class MeshComponent : public EntityComponent
{
public:
	enum VertexType { Static, Skeletal};

public:
	MeshComponent(const std::string &name, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault());
	virtual ~MeshComponent() = default;

public:
	virtual const void *GetVertices() const = 0;
	virtual size_t GetVerticesLength() const = 0;
	virtual VertexType GetVertexType() const = 0;
	const BufferLayout &GetVertexBufferLayout() const;

public:
	std::vector<uint32_t> indices;
	Ref<Material> material;
};
