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

	bool RayMeshIntersectionLocalSpace(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection);
	bool RayMeshIntersection(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection);

	std::vector<uint32_t> GetVerticesInRadiusLocalSpace(const Mesh *mesh, const glm::vec3 &point, float radius);
	std::vector<uint32_t> GetVerticesInRadius(const Mesh *mesh, const glm::vec3 &point, float radius);

    float BlendMix(const float oldWeight, const float goalWeight, const float alpha);
    float BlendAdd(const float oldWeight, const float goalWeight, const float alpha);
    float BlendSubtract(const float oldWeight, const float goalWeight, const float alpha);
    float BlendMultiply(const float oldWeight, const float goalWeight, const float alpha);
    float BlendLighten(const float oldWeight, const float goalWeight, const float alpha);
    float BlendDarken(const float oldWeight, const float goalWeight, const float alpha);
    float BlendColorDodge(float oldWeight, float goalWeight, float alpha);
    float BlendDifference(float oldWeight, float goalWeight, float alpha);
    float BlendScreen(float oldWeight, float goalWeight, float alpha);
    float BlendHardLight(float oldWeight, float goalWeight, float alpha);
    float BlendOverlay(float oldWeight, float goalWeight, float alpha);
    float BlendSoftLight(float oldWeight, float goalWeight, float alpha);
    float BlendExclusion(float oldWeight, float goalWeight, float alpha);
}
