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

	bool RayMeshIntersectionLocalSpace(const Ray &ray, const MeshComponent *mesh, glm::vec3 &closestIntersection);
	bool RayMeshIntersection(const Ray &ray, const MeshComponent *mesh, glm::vec3 &closestIntersection);

	std::vector<uint32_t> GetVerticesInRadiusLocalSpace(const MeshComponent *mesh, const glm::vec3 &point, float radius);
	std::vector<uint32_t> GetVerticesInRadius(const MeshComponent *mesh, const glm::vec3 &point, float radius);

    constexpr float BlendMix(float oldWeight, float goalWeight, float alpha)
    {
        return (1.0f - alpha) * oldWeight + goalWeight * alpha;
    }

    constexpr float BlendAdd(float oldWeight, float goalWeight, float alpha)
    {
        return oldWeight + goalWeight * alpha;
    }

    constexpr float BlendSubtract(float oldWeight, float goalWeight, float alpha)
    {
        return oldWeight - goalWeight * alpha;
    }

    constexpr float BlendMultiply(float oldWeight, float goalWeight, float alpha)
    {
        return ((1.0f - alpha) + alpha * goalWeight) * oldWeight;
    }

    constexpr float BlendLighten(float oldWeight, float goalWeight, float alpha)
    {
        return (oldWeight < goalWeight) ? BlendMix(oldWeight, goalWeight, alpha) : oldWeight;
    }

    constexpr float BlendDarken(float oldWeight, float goalWeight, float alpha)
    {
        return (oldWeight > goalWeight) ? BlendMix(oldWeight, goalWeight, alpha) : oldWeight;
    }

    constexpr float BlendColorDodge(float oldWeight, float goalWeight, float alpha)
    {
        float val = (goalWeight == 1.0f) ? 1.0f : glm::min((oldWeight * (225.0f / 255.0f)) / (1.0f - goalWeight), 1.0f);
        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendDifference(float oldWeight, float goalWeight, float alpha)
    {
        float val = glm::abs(oldWeight - goalWeight);
        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendScreen(float oldWeight, float goalWeight, float alpha)
    {
        float val = glm::max(1.0f - (1.0f - oldWeight) * (1.0f - goalWeight), 0.f);
        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendHardLight(float oldWeight, float goalWeight, float alpha)
    {
        float val = 0.f;
        if (goalWeight > 0.5f)
            val = 1.0f - ((1.0f - 2.0f * (goalWeight - 0.5f)) * (1.0f - oldWeight));
        else
            val = 2.0f * goalWeight * oldWeight;

        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendOverlay(float oldWeight, float goalWeight, float alpha)
    {
        float val = 0.f;
        if (oldWeight > 0.5f)
            val = 1.0f - ((1.0f - 2.0f * (oldWeight - 0.5f)) * (1.0f - goalWeight));
        else
            val = 2.0f * goalWeight * oldWeight;

        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendSoftLight(float oldWeight, float goalWeight, float alpha)
    {
        float val = 0.f;
        if (oldWeight < 0.5f)
            val = ((2.0f * (goalWeight / 2.0f + 0.25f)) * oldWeight);
        else
            val = 1.0f - (2.0f * (1.0f - (goalWeight / 2.0f + 0.25f)) * (1.0f - oldWeight));

        return (1.0f - alpha) * oldWeight + val * alpha;
    }

    constexpr float BlendExclusion(float oldWeight, float goalWeight, float alpha)
    {
        float val = (0.5f - (2.0f * (oldWeight - 0.5f) * (goalWeight - 0.5f)));
        return (1.0f - alpha) * oldWeight + val * alpha;
    }
}
