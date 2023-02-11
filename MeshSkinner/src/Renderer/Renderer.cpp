#include "pch.h"
#include "Renderer.h"

Ref<Camera> Renderer::activeCamera;

std::unordered_map<const Mesh *, const uint32_t> Renderer::meshes;

DrawCalls Renderer::staticMeshStaticDrawCalls;
DrawCalls Renderer::skeletalMeshStaticDrawCalls;

DrawCalls Renderer::staticMeshDynamicDrawCalls;
DrawCalls Renderer::skeletalMeshDynamicDrawCalls;

void Renderer::Init()
{
	staticMeshStaticDrawCalls = DrawCalls();
	staticMeshStaticDrawCalls = DrawCalls();

	skeletalMeshDynamicDrawCalls = DrawCalls();
	skeletalMeshDynamicDrawCalls = DrawCalls();
}

void Renderer::Submit(Ref<Entity> entity)
{
	SubmitStatic<StaticMesh, StaticVertex>(entity->GetComponents<StaticMesh>(), staticMeshStaticDrawCalls, StaticVertex::layout);
	SubmitStatic<SkeletalMesh, SkeletalVertex>(entity->GetComponents<SkeletalMesh>(), skeletalMeshStaticDrawCalls, SkeletalVertex::layout);
}

void Renderer::FrameBegin()
{

}

static void RenderDrawCalls(const Ref<Camera> &camera, const DrawCalls &drawCalls)
{
	for (const auto &[shader, vao] : drawCalls)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());

		vao->Bind();
		glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::FrameEnd()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderDrawCalls(activeCamera, staticMeshStaticDrawCalls);
	RenderDrawCalls(activeCamera, skeletalMeshStaticDrawCalls);
}
