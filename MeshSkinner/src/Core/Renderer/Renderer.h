#pragma once

#include "VertexArray.h"
#include "Core/Mesh/MeshComponent.h"
#include "Core/Mesh/StaticMeshComponent.h"
#include "Core/Mesh/SkeletalMeshComponent.h"
#include "Core/Entity/Transform.h"
#include "Core/Entity/Entity.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/Buffer/StorageBuffer.h"

struct VertexInfo // TODO: these could be just 2 separate vbos (one for transformIndex and another for skeletonBones and skeletonTransforms indices) - could also be 3 separate vbos which could save us unused data transfer (just add use what needed)
{
	VertexInfo(GLuint transformIndex = -1, GLuint skeletonBonesIndex = -1, GLuint skeletonTransformsIndex = -1);

	GLuint transformIndex;
	GLuint skeletonBonesIndex;
	GLuint skeletonTransformsIndex;

	static const BufferLayout &layout;
};

struct DrawCallInfo
{
	DrawCallInfo(const Ref<VertexArray<uint32_t>> &vao = MakeRef<VertexArray<uint32_t>>(), const Ref<StorageBuffer<glm::vec4>> &finalPos = MakeRef<StorageBuffer<glm::vec4>>());

	Ref<VertexArray<uint32_t>> vao;
	Ref<StorageBuffer<glm::vec4>> finalPos;
};

using DrawCalls = std::map<Ref<Shader>, DrawCallInfo>;

class Renderer
{
public:
	static void Init();

	static void Submit(Entity *entity);
	// TODO: add Remove which should be called upon destruction of MeshComponent owning entity - shouldn't just use weak_ptrs instead and clean the up accordingly?

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMesh(Entity *entity, const MeshComponent *mesh, DrawCalls &drawCalls);
	static void SubmitMesh(Entity *entity, const Ref<StaticMeshComponent> &mesh);
	static void SubmitMesh(Entity *entity, const Ref<SkeletalMeshComponent> &mesh);

	static void Render(const DrawCalls::iterator &it);

public:
	static void UpdateMeshVertices(const MeshComponent *mesh);
	static std::vector<glm::vec4> GetFinalVertPosData(const MeshComponent *mesh);

	static void UpdateBoneRadius(const SkeletalMeshComponent *mesh, float radius);

public:
	inline static Camera *activeCamera = nullptr;

	// TODO: these could/should be handled via materials
	inline static Entity *selectedEntity = nullptr;
	inline static uint32_t selectedBone = 0;

	inline static glm::vec3 color000 = glm::vec3(0.33f, 0.07f, 0.32f);
	inline static glm::vec3 color025 = glm::vec3(0.23f, 0.37f, 0.55f);
	inline static glm::vec3 color050 = glm::vec3(0.18f, 0.60f, 0.63f);
	inline static glm::vec3 color075 = glm::vec3(0.06f, 0.65f, 0.24f);
	inline static glm::vec3 color100 = glm::vec3(1.00f, 0.81f, 0.13f);

	// will be passed as uniform and can be used in shaders for rendering as semi-transparent
	inline static float finalAlpha = 0.6f;

private:
	inline static DrawCalls staticMeshDrawCalls;
	inline static DrawCalls skeletalMeshDrawCalls;

private:
	// key: entity already rendered, value: transform id to use for rendering
	inline static std::unordered_map<Entity *, const uint32_t> entities;

	// key: mesh already rendered, value: offset in vbo where the vertices start
	inline static std::unordered_map<const MeshComponent *, const uint32_t> meshes;

	// TODO: don't need unique ptrs? - these objects can be stored on stack (possibly issues with operator=?)
	inline static Unique<StorageBuffer<glm::mat4>> transforms;
	inline static Unique<StorageBuffer<BoneGPU>> bones;

	inline static uint32_t currentSkeletonBonesIndex = -1;
	inline static uint32_t currentSkeletonTransformsIndex = -1;
};
