#pragma once

#include "Application/Core.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Entity/Entity.h"
#include "Camera.h"

class Renderer
{
public:
	static void Init();

	static void Submit(Ref<Entity> entity);
	// TODO: add Remove which should be called opon destruction of Mesh owner

	static void FrameBegin();
	static void FrameEnd();

private:
	template <typename M, typename V>
	static void SubmitStatic(const std::unordered_set<M *> &submitMeshes, std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> &drawCalls, const BufferLayout &vertexLayout)
	{
		for (const auto &mesh : submitMeshes)
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
				if (drawCalls.find(mesh->material->shader) == drawCalls.end())
				{
					auto ibo = MakeRef<IndexBuffer<uint32_t>>();
					auto vbo = MakeRef<VertexBuffer<V>>(vertexLayout);
					auto vao = MakeRef<VertexArray<uint32_t>>();
					vao->SetVertexBuffer(vbo, 0);
					vao->SetIndexBuffer(ibo);
					drawCalls.insert({ mesh->material->shader, vao });
				}

				// append the data to the vbo and ibo of the vao for the current shader
				auto vbo = TypedVB<V>(drawCalls[mesh->material->shader]->GetVertexBuffer(0).get());

				// cache the current vbo length for ibo offset
				indexOffset = vbo->GetLength();

				// vbo - add vertices
				vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());

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
	}

public:
	static Ref<Camera> activeCamera;

private:
	// key: rendred mesh pointer, value: index offset to use
	static std::unordered_map<const Mesh *, const uint32_t> meshes;

	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> staticMeshStaticDrawCalls;
	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> skeletalMeshStaticDrawCalls;

	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> staticMeshDynamicDrawCalls;
	static std::unordered_map<Ref<Shader>, Ref<VertexArray<uint32_t>>> skeletalMeshDynamicDrawCalls;
};
