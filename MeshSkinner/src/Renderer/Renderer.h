#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Entity/Entity.h"
#include "Camera.h"

class Renderer
{
public:
	static void Init();

	static void Submit(Ref<Entity> entity);
	// TODO: add Remove which should be called opon destruction of Mesh owner

	static void FrameBegin();
	static void FrameEnd();

public:
	static Ref<Camera> activeCamera;

private:
	// key: rendred mesh pointer, value: index offset to use
	static std::unordered_map<const Mesh *, const uint32_t> meshes;

	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> staticMeshStaticDrawCalls;
	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> skeletalMeshStaticDrawCalls;

	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> staticMeshDynamicDrawCalls;
	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> skeletalMeshDynamicDrawCalls;
};
