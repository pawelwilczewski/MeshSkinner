#include "pch.h"
#include "MeshLibrary.h"

#include "tiny_gltf.h"

std::unordered_map<std::string, Ref<StaticMesh>> MeshLibrary::staticMeshes;
std::unordered_map<std::string, std::pair<Ref<Skeleton>, Ref<SkeletalMesh>>> MeshLibrary::skeletalMeshes;

Ref<StaticMesh> MeshLibrary::GetCube()
{
	if (staticMeshes.find("Cube") == staticMeshes.end())
	{
		std::vector<StaticVertex> vertices;
		vertices.push_back(StaticVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));

		std::vector<uint32_t> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		staticMeshes.insert({ "Cube", MakeRef<StaticMesh>(vertices, indices, MaterialLibrary::GetDefault(), true) });
	}

	return staticMeshes["Cube"];
}

static tinygltf::Model model;
static tinygltf::TinyGLTF loader;
static std::string err;
static std::string warn;

static bool LoadGLTF(const std::string &path)
{
	bool success = false;
	auto extension = std::filesystem::path(path).extension();

	if (extension == ".gltf")		success = loader.LoadASCIIFromFile(&model, &err, &warn, path);
	else if (extension == ".glb")	success = loader.LoadBinaryFromFile(&model, &err, &warn, path);

	if (!warn.empty())	Log::Error("GLTF Import: Warning: {}", path);
	if (!err.empty())	Log::Error("GLTF Import: Error: {}", path);
	if (!success)		Log::Error("GLTF Import: Failed to parse from file: {}", path);

	return success;
}

static bool LoadFromFile(const std::string &path)
{
	auto extension = std::filesystem::path(path).extension();

	if (extension == ".gltf" || extension == ".glb")
	{
		if (!LoadGLTF(path))
			return false;
	}
	else
	{
		Log::Error("Unsupported mesh import file extension: {}", extension);
		return false;
	}

	return true;
}

bool MeshLibrary::Get(const std::string &path, Ref<StaticMesh> &outMesh)
{
	// TODO: get from the cache map if already loaded once

	if (!LoadFromFile(path))
		return false;
	
	// for now, we just assume the imported file is .gltf
	for (const auto &mesh : model.meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// add indices
			auto &indices = model.accessors[primitive.indices];
			void *indicesData = &(model.buffers[model.bufferViews[indices.bufferView].buffer].data[indices.byteOffset + model.bufferViews[indices.bufferView].byteOffset]);

			switch (indices.componentType)
			{
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			{
				uint8_t *buffer = static_cast<uint8_t *>(indicesData);
				for (size_t i = 0; i < indices.count; ++i)
					outMesh->indices.push_back(static_cast<uint32_t>(buffer[i]));
				break;
			}
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			{
				uint16_t *buffer = static_cast<uint16_t *>(indicesData);
				for (size_t i = 0; i < indices.count; ++i)
					outMesh->indices.push_back(static_cast<uint32_t>(buffer[i]));
				break;
			}
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			{
				uint32_t *buffer = static_cast<uint32_t *>(indicesData);
				for (size_t i = 0; i < indices.count; ++i)
					outMesh->indices.push_back(buffer[i]);
				break;
			}
			}

			// attributes
			for (const auto &[attribute, index] : primitive.attributes)
			{
				
				//model.accessors[index];
			}
		}
	}

	return true;
}

bool MeshLibrary::Get(const std::string &path, Ref<Skeleton> &outSkeleton, Ref<SkeletalMesh> &outMesh)
{
	if (!LoadFromFile(path))
		return false;

	// for now, we just assume the imported file is .gltf

	return true;
}
