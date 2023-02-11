#include "pch.h"
#include "Renderer.h"

Ref<Camera> Renderer::activeCamera;

std::unordered_map<const Mesh *, const uint32_t> Renderer::meshes;

std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> Renderer::staticMeshStaticDrawCalls;
std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> Renderer::skeletalMeshStaticDrawCalls;

std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>> Renderer::staticMeshDynamicDrawCalls;
std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>> Renderer::skeletalMeshDynamicDrawCalls;

void Renderer::Init()
{
	staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>>();
	staticMeshStaticDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<StaticVertex, uint32_t>>>();

	skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>>();
	skeletalMeshDynamicDrawCalls = std::unordered_map<Ref<Shader>, Ref<VertexArray<SkeletalVertex, uint32_t>>>();
}

void Renderer::Submit(Ref<Entity> entity)
{
	// add the static static meshes
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	for (const auto &mesh : staticMeshes)
	{
		if (!mesh->isStatic) continue;
		
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
			if (staticMeshStaticDrawCalls.find(mesh->material->shader) == staticMeshStaticDrawCalls.end())
			{
				auto ibo = MakeRef<IndexBuffer<uint32_t>>();
				auto vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
				auto vao = MakeRef<VertexArray<StaticVertex, uint32_t>>();
				vao->SetVertexBuffer(vbo, 0);
				vao->SetIndexBuffer(ibo);
				staticMeshStaticDrawCalls.insert({ mesh->material->shader, vao });
			}

			// append the data to the vbo and ibo of the vao for the current shader
			auto &vbo = staticMeshStaticDrawCalls[mesh->material->shader]->GetVertexBuffer(0);

			// cache the current vbo length for ibo offset
			indexOffset = vbo->GetLength();

			// vbo - add vertices
			vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());

			// add the meshes to the map for reuse later
			meshes.insert({ mesh, indexOffset });
		}

		auto &ibo = staticMeshStaticDrawCalls[mesh->material->shader]->GetIndexBuffer();

		// add indices (appropriately offset)
		std::vector<uint32_t> indicesOffset = mesh->indices;
		if (indexOffset > 0)
			for (auto &index : indicesOffset)
				index += indexOffset;

		ibo->SetData(indicesOffset.data(), indicesOffset.size(), ibo->GetLength());
	}
	// add the static skeletal meshes
	//auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
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
}
