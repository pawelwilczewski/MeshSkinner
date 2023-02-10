#include "pch.h"
#include "Renderer.h"

std::unordered_set<Ref<Entity>> Renderer::entities;

//std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> Renderer::staticMeshStaticDrawCalls;
//std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> Renderer::skeletalMeshStaticDrawCalls;

//std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> Renderer::staticMeshDynamicDrawCalls;
//std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> Renderer::skeletalMeshDynamicDrawCalls;

void Renderer::Init()
{
	//staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>>();
	//staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>>();

	//skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>>();
	//skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>>();
}

void Renderer::Submit(Ref<Entity> entity)
{
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
}

void Renderer::FrameBegin()
{

}

void Renderer::FrameEnd()
{

}
