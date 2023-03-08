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
	std::unordered_map<Ref<Entity>, const uint32_t> entities; // TODO: use weak ptrs for entities
	// key: skeleton already rendered, value: id to use for bones transforms (start)
	std::unordered_map<Ref<Skeleton>, const uint32_t> skeletons; // TODO: use weak ptrs for skeletons
	std::unordered_map<const Mesh *, const uint32_t> meshes; // TODO: use weak ptrs for meshes
	Unique<StorageBuffer<glm::mat4>> transforms;
	Unique<StorageBuffer<BoneGPU>> bones;
	Unique<StorageBuffer<MaterialGPU>> materials;
	Unique<StorageBuffer<VertexInfo>> vertexInfo;
};

using DrawCalls = std::map<Ref<Shader>, Ref<DrawCallInfo>>;

class Renderer
{
public:
	static void Init();

	static void Submit(const Ref<Entity> &entity);
	// TODO: add Remove which should be called upon destruction of Mesh owning entity - shouldn't just use weak_ptrs instead and clean the up accordingly?

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMesh(const Ref<Entity> &entity, const Mesh *mesh, DrawCalls &drawCalls, bool skeletal = false);
	static void SubmitMesh(const Ref<Entity> &entity, const Ref<StaticMesh> &mesh);
	static void SubmitMesh(const Ref<Entity> &entity, const Ref<SkeletalMesh> &mesh);

	static void Render(const DrawCalls::iterator &it);

public:
	static void UpdateMeshVertices(const Mesh *mesh);

public:
	static Ref<Camera> activeCamera;
	static int activeBone;

private:
	static DrawCalls staticMeshDrawCalls;
	static DrawCalls skeletalMeshDrawCalls;
};
