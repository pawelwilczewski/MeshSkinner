#pragma once

#include "Application/Core.h"
#include "Vertex.h"
#include "Material.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh() = default;

public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	Ref<Material> material;
};
