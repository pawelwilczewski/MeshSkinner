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

bool MathUtils::RayMeshIntersectionLocalSpace(const Ray &ray, const MeshComponent *mesh, glm::vec3 &closestIntersection)
{
    float smallestDistance = -1.f;

    const auto &layout = mesh->GetVertexBufferLayout();
    auto stride = layout.GetStride();
    auto offset = layout["position"].offset;

    const auto &matrix = mesh->GetEntity()->GetWorldMatrix();
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

bool MathUtils::RayMeshIntersection(const Ray &ray, const MeshComponent *mesh, glm::vec3 &closestIntersection)
{
    auto result = RayMeshIntersectionLocalSpace(ray, mesh, closestIntersection);

    const auto &matrix = mesh->GetEntity()->GetWorldMatrix();
    closestIntersection = matrix * glm::vec4(closestIntersection, 1.f);

    return result;
}

std::vector<uint32_t> MathUtils::GetVerticesInRadiusLocalSpace(const MeshComponent *mesh, const glm::vec3 &point, float radius)
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

std::vector<uint32_t> MathUtils::GetVerticesInRadius(const MeshComponent *mesh, const glm::vec3 &point, float radius)
{
    const auto &invMatrix = glm::inverse(mesh->GetEntity()->GetWorldMatrix());
    auto pointLocalSpace = glm::vec3(invMatrix * glm::vec4(point, 1.f));

    return GetVerticesInRadiusLocalSpace(mesh, pointLocalSpace, radius);
}
