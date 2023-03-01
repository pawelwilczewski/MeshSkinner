#pragma once

#include "Application/Core.h"

namespace MathUtils
{
	struct Ray
	{
		Ray(const glm::vec3 &origin = glm::vec3(0.f), const glm::vec3 &direction = glm::vec3(0.f));

		glm::vec3 origin;
		glm::vec3 direction;

	public:
		bool IntersectsTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, glm::vec3 &intersectionPoint) const;
	};

	bool RayMeshIntersection(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection);

	bool RayMeshIntersection(const Ray &ray, const Ref<SkeletalMesh> &mesh, glm::vec3 &intersection);

	bool RayMeshIntersection(const Ray &ray, const Ref<StaticMesh> &mesh, glm::vec3 &intersection);
}
