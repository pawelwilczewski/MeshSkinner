#pragma once

#include "Application/Core.h"
#include "VertexArray.h"
#include "Core/Mesh/Mesh.h"
#include "Core/Entity/Transform.h"
#include "Core/Entity/Entity.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/Buffer/StorageBuffer.h"

struct VertexInfo
{
	VertexInfo(uint32_t transformID, uint32_t materialID);

	uint32_t transformID;
	uint32_t materialID;
};

struct DrawCallInfo
{
	DrawCallInfo();

	Unique<VertexArray<uint32_t>> vao;
	// key: entity already rendered, value: transform id to use for mesh
	std::unordered_map<Ref<Entity>, const uint32_t> entities;
	// key: skeleton already rendered, value: transform id to use for bones (start)
	std::unordered_map<Ref<Skeleton>, const uint32_t> skeletons;
	Unique<StorageBuffer<glm::mat4>> transforms;
	Unique<StorageBuffer<MaterialGPU>> materials;
	Unique<StorageBuffer<VertexInfo>> vertexInfo;
};

using DrawCalls = std::unordered_map<Ref<Shader>, Ref<DrawCallInfo>>;

class Renderer
{
public:
	static void Init();

	static void Submit(const Ref<Entity> &entity);
	// TODO: add Remove which should be called opon destruction of Mesh owner

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMeshStatic(const Ref<Entity> &entity, const Mesh *mesh, DrawCalls &drawCalls, std::function<void(VertexArray<uint32_t> &)> vaoInitFunction, std::function<void(VertexArray<uint32_t> &)> fillVertexBufferFunction);

	void Renderer::SubmitBoneStatic(const Ref<Entity> &entity, const Bone *bone, const Ref<Shader> &shader, DrawCalls &drawCalls, std::function<void(VertexArray<uint32_t> &)> vaoInitFunction, std::function<void(VertexArray<uint32_t> &)> fillVertexBufferFunction);

	static void SubmitMeshStatic(const Ref<Entity> &entity, const Ref<StaticMesh> &mesh);
	static void SubmitMeshStatic(const Ref<Entity> &entity, const Ref<SkeletalMesh> &mesh);

	static void RenderDrawCalls(const Ref<Camera> &camera, const DrawCalls &drawCalls);

public:
	static Ref<Camera> activeCamera;

private:
	static DrawCalls staticMeshDrawCallsStatic;
	static DrawCalls skeletalMeshDrawCallsStatic;

	static DrawCalls staticMeshDrawCallsDynamic;
	static DrawCalls skeletalMeshDrawCallsDynamic;
};