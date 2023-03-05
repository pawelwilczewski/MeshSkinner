#include "pch.h"
#include "MathUtils.h"

MathUtils::Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction) : origin(origin), direction(direction)
{
}

bool MathUtils::Ray::IntersectsTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, glm::vec3 &intersectionPoint) const
{
    glm::vec2 uv;
    float distance;
    auto hit = glm::intersectRayTriangle(origin, direction, v0, v1, v2, uv, distance);
    intersectionPoint = origin + direction * distance;
    return hit;
}

bool MathUtils::RayMeshIntersectionLocalSpace(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection)
{
    float smallestDistance = -1.f;

    const auto &layout = mesh->GetVertexBufferLayout();
    auto stride = layout.GetStride();
    auto offset = layout["position"].offset;

    const auto &matrix = mesh->GetEntity().lock()->GetWorldMatrix();
    auto invMatrix = glm::inverse(matrix);
    auto localRay = Ray(glm::vec3(invMatrix * glm::vec4(ray.origin, 1.f)), glm::mat3(invMatrix) * ray.direction);

    auto vertBytes = (uint8_t *)mesh->GetVertices();
    for (size_t i = 0; i < mesh->indices.size(); i += 3)
    {
        auto v0 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 0] * stride + offset);
        auto v1 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 1] * stride + offset);
        auto v2 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 2] * stride + offset);

        // intersection check and update closest intersection
        glm::vec3 localIntersection;
        if (localRay.IntersectsTriangle(v0, v1, v2, localIntersection))
        {
            auto distance = glm::distance(localRay.origin, localIntersection);
            if (smallestDistance >= 0.f)
            {
                if (distance < smallestDistance)
                {
                    closestIntersection = localIntersection;
                    smallestDistance = distance;
                }
            }
            else
            {
                closestIntersection = localIntersection;
                smallestDistance = distance;
            }
        }
    }

    return smallestDistance >= 0.f;
}

bool MathUtils::RayMeshIntersection(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection)
{
    auto result = RayMeshIntersectionLocalSpace(ray, mesh, closestIntersection);

    const auto &matrix = mesh->GetEntity().lock()->GetWorldMatrix();
    closestIntersection = matrix * glm::vec4(closestIntersection, 1.f);

    return result;
}

std::vector<uint32_t> MathUtils::GetVerticesInRadiusLocalSpace(const Mesh *mesh, const glm::vec3 &point, float radius)
{
    std::vector<uint32_t> result;

    const auto &layout = mesh->GetVertexBufferLayout();
    auto stride = layout.GetStride();
    auto offset = layout["position"].offset;

    auto vertBytes = (uint8_t *)mesh->GetVertices();
    for (size_t i = 0; i < mesh->GetVerticesLength(); i++)
    {
        auto v = *(glm::vec3 *)(vertBytes + i * stride + offset);

        if (glm::distance(point, v) <= radius)
            result.push_back((uint32_t)i);
    }

    return result;
}

std::vector<uint32_t> MathUtils::GetVerticesInRadius(const Mesh *mesh, const glm::vec3 &point, float radius)
{
    const auto &invMatrix = glm::inverse(mesh->GetEntity().lock()->GetWorldMatrix());
    auto pointLocalSpace = glm::vec3(invMatrix * glm::vec4(point, 1.f));

    return GetVerticesInRadiusLocalSpace(mesh, pointLocalSpace, radius);
}

float MathUtils::BlendMix(float oldWeight, float goalWeight, float alpha)
{
    return (1.0f - alpha) * oldWeight + goalWeight * alpha;
}

float MathUtils::BlendAdd(float oldWeight, float goalWeight, float alpha)
{
    return oldWeight + goalWeight * alpha;
}

float MathUtils::BlendSubtract(float oldWeight, float goalWeight, float alpha)
{
    return oldWeight - goalWeight * alpha;
}

float MathUtils::BlendMultiply(float oldWeight, float goalWeight, float alpha)
{
    return ((1.0f - alpha) + alpha * goalWeight) * oldWeight;
}

float MathUtils::BlendLighten(float oldWeight, float goalWeight, float alpha)
{
    return (oldWeight < goalWeight) ? BlendMix(oldWeight, goalWeight, alpha) : oldWeight;
}

float MathUtils::BlendDarken( float oldWeight,  float goalWeight,  float alpha)
{
    return (oldWeight > goalWeight) ? BlendMix(oldWeight, goalWeight, alpha) : oldWeight;
}

float MathUtils::BlendColorDodge(float oldWeight, float goalWeight, float alpha)
{
    float val = (goalWeight == 1.0f) ? 1.0f : glm::min((oldWeight * (225.0f / 255.0f)) / (1.0f - goalWeight), 1.0f);
    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendDifference(float oldWeight, float goalWeight, float alpha)
{
    float val = fabsf(oldWeight - goalWeight);
    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendScreen(float oldWeight, float goalWeight, float alpha)
{
    float val = glm::max(1.0f - (1.0f - oldWeight) * (1.0f - goalWeight), 0.f);
    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendHardLight(float oldWeight, float goalWeight, float alpha)
{
    float val;
    if (goalWeight > 0.5f)
        val = 1.0f - ((1.0f - 2.0f * (goalWeight - 0.5f)) * (1.0f - oldWeight));
    else
        val = 2.0f * goalWeight * oldWeight;

    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendOverlay(float oldWeight, float goalWeight, float alpha)
{
    float val;
    if (oldWeight > 0.5f)
        val = 1.0f - ((1.0f - 2.0f * (oldWeight - 0.5f)) * (1.0f - goalWeight));
    else
        val = 2.0f * goalWeight * oldWeight;

    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendSoftLight(float oldWeight, float goalWeight, float alpha)
{
    float val;
    if (oldWeight < 0.5f)
        val = ((2.0f * (goalWeight / 2.0f + 0.25f)) * oldWeight);
    else
        val = 1.0f - (2.0f * (1.0f - (goalWeight / 2.0f + 0.25f)) * (1.0f - oldWeight));

    return (1.0f - alpha) * oldWeight + val * alpha;
}

float MathUtils::BlendExclusion(float oldWeight, float goalWeight, float alpha)
{
    float val = (0.5f - (2.0f * (oldWeight - 0.5f) * (goalWeight - 0.5f)));
    return (1.0f - alpha) * oldWeight + val * alpha;
}
