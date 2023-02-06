#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"

class Renderer
{
public:
	static void Init();

	static void Submit(const Mesh &mesh);
	static void Submit(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, Ref<Material> material);

	static void Reset();
	static void Render();

private:
	//static std::unordered_map<Shader, VertexArray> drawCalls;
};
