#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Entity/Entity.h"
#include "Camera.h"
#include "StorageBuffer.h"

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
	// key: mesh already rendered, value: ibo index offset to use if mesh is reused
	std::unordered_map<const Mesh *, const uint32_t> meshes;
	Unique<StorageBuffer<glm::mat4>> transforms;
	Unique<StorageBuffer<MaterialGPU>> materials;
	Unique<StorageBuffer<VertexInfo>> vertexInfo;
};

using DrawCalls = std::unordered_map<Ref<Shader>, Ref<DrawCallInfo>>;

class Renderer
{
public:
	static void Init();

	static void Submit(Ref<Entity> entity);
	// TODO: add Remove which should be called opon destruction of Mesh owner

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMeshStatic(Ref<Entity> entity, const Mesh *mesh, DrawCalls &drawCalls, std::function<void(VertexArray<uint32_t> &)> vaoInitFunction, std::function<uint32_t(VertexArray<uint32_t> &)> fillVertexBufferFunction);

	static void RenderDrawCalls(const Ref<Camera> &camera, const DrawCalls &drawCalls);

public:
	static Ref<Camera> activeCamera;

private:
	static DrawCalls staticMeshDrawCallsStatic;
	static DrawCalls skeletalMeshDrawCallsStatic;

	static DrawCalls staticMeshDrawCallsDynamic;
	static DrawCalls skeletalMeshDrawCallsDynamic;
};
