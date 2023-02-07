#pragma once

#include "Application/Core.h"
#include "Vertex.h"
#include "Material.h"

class Mesh
{
public:
	Mesh(const std::vector<uint32_t> indices, Ref<Material> material);
	virtual ~Mesh() = default;

public:
	std::vector<uint32_t> indices;
	Ref<Material> material;
};

class StaticMesh : public Mesh
{
public:
	StaticMesh(const std::vector<StaticVertex> vertices, const std::vector<uint32_t> indices, Ref<Material> material);
	virtual ~StaticMesh() = default;

public:
	std::vector<StaticVertex> vertices;
};

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh(const std::vector<SkeletalVertex> vertices, const std::vector<uint32_t> indices, Ref<Material> material);
	virtual ~SkeletalMesh() = default;

public:
	std::vector<SkeletalVertex> vertices;
};
