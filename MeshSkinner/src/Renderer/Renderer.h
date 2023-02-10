#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Entity/Entity.h"

class Renderer
{
public:
	static void Init();

	static void Submit(Ref<Entity> entity);

	static void FrameBegin();
	static void FrameEnd();

private:
	static std::unordered_set<Ref<Entity>> entities;

	//static std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> staticMeshStaticDrawCalls;
	//static std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> skeletalMeshStaticDrawCalls;

	//static std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> staticMeshDynamicDrawCalls;
	//static std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> skeletalMeshDynamicDrawCalls;
};
