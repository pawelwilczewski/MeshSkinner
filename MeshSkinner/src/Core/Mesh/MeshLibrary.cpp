#include "pch.h"
#include "MeshLibrary.h"

std::unordered_map<std::string, Ref<StaticMesh>> MeshLibrary::staticMeshes;
std::unordered_map<std::string, Ref<SkeletalMesh>> MeshLibrary::skeletalMeshes;

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
