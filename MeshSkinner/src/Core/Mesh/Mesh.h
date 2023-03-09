#pragma once

#include "Application/Core.h"
#include "Vertex.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Renderer/Material/MaterialLibrary.h"
#include "Core/Mesh/Skeleton.h"

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
	Ref<Skeleton> skeleton;
	std::vector<SkeletalVertex> vertices;
};
