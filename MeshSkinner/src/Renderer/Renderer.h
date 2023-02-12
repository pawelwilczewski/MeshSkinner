#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Entity/Entity.h"
#include "Camera.h"

using DrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>>;

class Renderer
{
public:
	static void Init();

	static void Submit(Ref<Entity> entity);
	// TODO: add Remove which should be called opon destruction of Mesh owner

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMeshStatic(Mesh *mesh, DrawCalls &drawCalls, std::function<void(const Ref<VertexArray<uint32_t>> &)> vaoInitFunction, std::function<uint32_t(const Ref<VertexArray<uint32_t>> &)> fillVertexBufferFunction);

public:
	static Ref<Camera> activeCamera;

private:
	// key: rendered mesh pointer, value: index offset to use if mesh is reused
	static std::unordered_map<const Mesh *, const uint32_t> meshes;

	static DrawCalls staticMeshStaticDrawCalls;
	static DrawCalls skeletalMeshStaticDrawCalls;

	static DrawCalls staticMeshDynamicDrawCalls;
	static DrawCalls skeletalMeshDynamicDrawCalls;
};
