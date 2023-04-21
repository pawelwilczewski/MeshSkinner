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

bool MathUtils::RayMeshIntersection(const Ray &ray, const MeshComponent *mesh, glm::vec3 &closestIntersection)
{
    return RayMeshIntersection(ray, Renderer::GetFinalVertPosData(mesh), mesh->indices, closestIntersection);
}

bool MathUtils::RayMeshIntersection(const Ray &ray, const std::vector<glm::vec4> &verts, const std::vector<uint32_t> &indices, glm::vec3 &closestIntersection)
{
    float smallestDistance = -1.f;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        const auto &v0 = verts[indices[i + 0]];
        const auto &v1 = verts[indices[i + 1]];
        const auto &v2 = verts[indices[i + 2]];

        // intersection check and update closest intersection
        glm::vec3 intersection;
        if (ray.IntersectsTriangle(v0, v1, v2, intersection))
        {
            auto distance = glm::distance(ray.origin, intersection);
            if (smallestDistance >= 0.f)
            {
                if (distance < smallestDistance)
                {
                    closestIntersection = intersection;
                    smallestDistance = distance;
                }
            }
            else
            {
                closestIntersection = intersection;
                smallestDistance = distance;
            }
        }
    }

    return smallestDistance >= 0.f;
}

std::vector<uint32_t> MathUtils::GetVerticesInRadius(const MeshComponent *mesh, const glm::vec3 &point, float radius)
{
    return GetVerticesInRadius(Renderer::GetFinalVertPosData(mesh), point, radius);
}

std::vector<uint32_t> MathUtils::GetVerticesInRadius(const std::vector<glm::vec4> &verts, const glm::vec3 &point, float radius)
{
    std::vector<uint32_t> result;

    for (size_t i = 0; i < verts.size(); i++)
        if (glm::distance(point, glm::vec3(verts[i])) <= radius)
            result.push_back((uint32_t)i);

    return result;
}
