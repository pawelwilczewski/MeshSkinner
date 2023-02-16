#pragma once

#include "Application/Core.h"
#include "Vertex.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Renderer/Material/MaterialLibrary.h"

class Mesh : public EntityComponent
{
public:
	Mesh(const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault(), bool isStatic = true);
	virtual ~Mesh() = default;

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
	std::vector<StaticVertex> vertices;
};

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh(const std::vector<SkeletalVertex> &vertices = {}, const std::vector<uint32_t> &indices = {}, Ref<Material> material = MaterialLibrary::GetDefault(), bool isStatic = true);
	virtual ~SkeletalMesh() = default;

public:
	std::vector<SkeletalVertex> vertices;
};
