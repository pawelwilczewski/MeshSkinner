#include "pch.h"
#include "Renderer.h"

Ref<Camera> Renderer::activeCamera;

std::unordered_map<const Mesh *, const uint32_t> Renderer::meshes;

std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> Renderer::staticMeshStaticDrawCalls;
std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> Renderer::skeletalMeshStaticDrawCalls;

std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> Renderer::staticMeshDynamicDrawCalls;
std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> Renderer::skeletalMeshDynamicDrawCalls;

void Renderer::Init()
{
	staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>>();
	staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>>();

	skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>>();
	skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>>();
}

void Renderer::Submit(Ref<Entity> entity)
{
	SubmitStatic<StaticMesh, StaticVertex>(entity->GetComponents<StaticMesh>(), staticMeshStaticDrawCalls, StaticVertex::layout);
	SubmitStatic<SkeletalMesh, SkeletalVertex>(entity->GetComponents<SkeletalMesh>(), skeletalMeshStaticDrawCalls, SkeletalVertex::layout);
}

void Renderer::FrameBegin()
{

}

void Renderer::FrameEnd()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto &[shader, vao] : staticMeshStaticDrawCalls)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", activeCamera->GetViewProjectionMatrix());

		vao->Bind();
		glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
	}
	for (const auto &[shader, vao] : skeletalMeshStaticDrawCalls)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", activeCamera->GetViewProjectionMatrix());

		vao->Bind();
		glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
	}
}
