#pragma once

#include "Application/Core.h"
#include "VertexArray.h"
#include "Core/Mesh/MeshComponent.h"
#include "Core/Mesh/StaticMeshComponent.h"
#include "Core/Mesh/SkeletalMeshComponent.h"
#include "Core/Entity/Transform.h"
#include "Core/Entity/Entity.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/Buffer/StorageBuffer.h"

struct VertexInfo
{
	VertexInfo(uint32_t transformID, uint32_t materialID, uint32_t skeletonID = -1);

	uint32_t transformID;
	uint32_t materialID;
	uint32_t skeletonID;
};

struct DrawCallInfo
{
	DrawCallInfo();

	Unique<VertexArray<uint32_t>> vao;
	// key: entity already rendered, value: transform id to use for mesh
	std::unordered_map<Entity *, const uint32_t> entities;
	// key: skeleton already rendered, value: id to use for bones transforms (start)
	std::unordered_map<Skeleton *, const uint32_t> skeletons;
	// key: mesh already rendered, value: offset in vbo where the vertices start
	std::unordered_map<const MeshComponent *, const uint32_t> meshes;
	Unique<StorageBuffer<glm::mat4>> transforms;
	Unique<StorageBuffer<BoneGPU>> bones;
	Unique<StorageBuffer<MaterialGPU>> materials;
	Unique<StorageBuffer<VertexInfo>> vertexInfo;
	Unique<StorageBuffer<glm::vec4>> finalPos;
};

using DrawCalls = std::map<Ref<Shader>, Ref<DrawCallInfo>>; // TODO: now: why Ref<DrawCallInfo> and not DrawCallInfo??

class Renderer
{
public:
	static void Init();

	static void Submit(Entity *entity);
	// TODO: add Remove which should be called upon destruction of MeshComponent owning entity - shouldn't just use weak_ptrs instead and clean the up accordingly?

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMesh(Entity *entity, const MeshComponent *mesh, DrawCalls &drawCalls, bool skeletal = false);
	static void SubmitMesh(Entity *entity, const Ref<StaticMeshComponent> &mesh);
	static void SubmitMesh(Entity *entity, const Ref<SkeletalMeshComponent> &mesh);

	static void Render(const DrawCalls::iterator &it);

public:
	static void UpdateMeshVertices(const MeshComponent *mesh);

	static std::vector<glm::vec4> GetFinalVertPosData(const MeshComponent *mesh);

public:
	static Camera *activeCamera;
	static int activeBone;

private:
	static DrawCalls staticMeshDrawCalls;
	static DrawCalls skeletalMeshDrawCalls;
};
