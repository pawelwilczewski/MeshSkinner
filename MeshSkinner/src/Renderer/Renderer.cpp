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

void Renderer::SubmitMeshStatic(Mesh *mesh, DrawCalls &drawCalls, std::function<void(const Ref<VertexArray<uint32_t>> &)> vaoInitFunction, std::function<uint32_t(const Ref<VertexArray<uint32_t>> &)> fillVertexBufferFunction)
{
	if (!mesh->isStatic) return;

	uint32_t indexOffset;
	// the mesh is already rendered - reuse its data
	if (meshes.find(mesh) != meshes.end())
	{
		// reuse the cached index offset
		indexOffset = meshes[mesh];
	}
	// new mesh
	else
	{
		// new shader - create an appropriate vao
		if (drawCalls.find(mesh->material->shader) == drawCalls.end())
		{
			auto vao = MakeRef<VertexArray<uint32_t>>();
			vaoInitFunction(vao);

			drawCalls.insert({ mesh->material->shader, vao });
		}

		indexOffset = fillVertexBufferFunction(drawCalls[mesh->material->shader]);

		// add the meshes to the map for reuse later
		meshes.insert({ mesh, indexOffset });
	}

	// add indices (appropriately offset)
	std::vector<uint32_t> indicesOffset = mesh->indices;
	if (indexOffset > 0)
		for (auto &index : indicesOffset)
			index += indexOffset;

	auto &ibo = drawCalls[mesh->material->shader]->GetIndexBuffer();
	ibo->SetData(indicesOffset.data(), indicesOffset.size(), ibo->GetLength());
}

void Renderer::Submit(Ref<Entity> entity)
{
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	for (auto mesh : staticMeshes)
	{
		SubmitMeshStatic(mesh, staticMeshStaticDrawCalls,
			[&](const Ref<VertexArray<uint32_t>> &vao)
			{
				auto ibo = MakeRef<IndexBuffer<uint32_t>>();
				auto vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
				vao->SetVertexBuffer(vbo, 0);
				vao->SetIndexBuffer(ibo);
			},
			[&](const Ref<VertexArray<uint32_t>> &vao) -> uint32_t
			{
				// append the data to the vbo
				auto vbo = TypedVB<StaticVertex>(vao->GetVertexBuffer(0).get());

				// cache the current vbo length for ibo offset
				auto indexOffset = vbo->GetLength();

				// vbo - add vertices
				vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());

				return indexOffset;
			});
	}
	
	auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
	for (auto mesh : skeletalMeshes)
	{
		SubmitMeshStatic(mesh, skeletalMeshStaticDrawCalls,
			[&](const Ref<VertexArray<uint32_t>> &vao)
			{
				auto ibo = MakeRef<IndexBuffer<uint32_t>>();
				auto vbo = MakeRef<VertexBuffer<SkeletalVertex>>(SkeletalVertex::layout);
				vao->SetVertexBuffer(vbo, 0);
				vao->SetIndexBuffer(ibo);
			},
			[&](const Ref<VertexArray<uint32_t>> &vao) -> uint32_t
			{
				// append the data to the vbo
				auto vbo = TypedVB<SkeletalVertex>(vao->GetVertexBuffer(0).get());

			// cache the current vbo length for ibo offset
			auto indexOffset = vbo->GetLength();

			// vbo - add vertices
			vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());

			return indexOffset;
			});
	}
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
