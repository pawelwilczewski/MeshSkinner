#pragma once

#include "Application/Core.h"
#include "Vertex.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Renderer/Material/MaterialLibrary.h"
#include "Core/Mesh/Skeleton.h"

class Mesh : public EntityComponent
{
public:
	enum VertexType { Static, Skeletal};

public:
	Mesh(const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault(), bool isStatic = true);
	virtual ~Mesh() = default;

public:
	virtual const void *GetVertices() const = 0;
	virtual VertexType GetVertexType() const = 0;
	const BufferLayout &GetVertexBufferLayout() const;

public:
	std::vector<uint32_t> indices;
	Ref<Material> material;
	bool isStatic;
};

class StaticMesh : public Mesh
{
public:
	StaticMesh(const std::vector<StaticVertex> &vertices = {}, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault(), bool isStatic = true);
	virtual ~StaticMesh() = default;

public:
	virtual const void *GetVertices() const override;
	virtual VertexType GetVertexType() const override;

public:
	std::vector<StaticVertex> vertices;
};

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh(const std::vector<SkeletalVertex> &vertices = {}, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault(), bool isStatic = true);
	virtual ~SkeletalMesh() = default;

public:
	virtual const void *GetVertices() const override;
	virtual VertexType GetVertexType() const override;

public:
	Ref<Skeleton> skeleton;
	std::vector<SkeletalVertex> vertices;
};
