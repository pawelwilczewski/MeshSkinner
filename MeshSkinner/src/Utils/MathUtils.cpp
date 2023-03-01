#include "pch.h"
#include "MathUtils.h"

namespace MathUtils
{
    Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction) : origin(origin), direction(direction)
    {
    }

    bool Ray::IntersectsTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, glm::vec3 &intersectionPoint) const
    {
        glm::vec2 uv;
        float distance;
        auto hit = glm::intersectRayTriangle(origin, direction, v0, v1, v2, uv, distance);
        intersectionPoint = origin + direction * distance;
        return hit;
    }

    bool MathUtils::RayMeshIntersection(const Ray &ray, const Mesh *mesh, glm::vec3 &closestIntersection)
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
                        closestIntersection = glm::vec3(matrix * glm::vec4(localIntersection, 1.f));
                        smallestDistance = distance;
                    }
                }
                else
                {
                    closestIntersection = glm::vec3(matrix * glm::vec4(localIntersection, 1.f));
                    smallestDistance = distance;
                }
            }
        }

        return smallestDistance >= 0.f;
    }

    bool MathUtils::RayMeshIntersection(const Ray &ray, const Ref<SkeletalMesh> &mesh, glm::vec3 &intersection)
    {
        return RayMeshIntersection(ray, mesh.get(), intersection);
    }

    bool MathUtils::RayMeshIntersection(const Ray &ray, const Ref<StaticMesh> &mesh, glm::vec3 &intersection)
    {
        return RayMeshIntersection(ray, mesh.get(), intersection);
    }

    std::vector<uint32_t> GetVerticesInRadius(const Mesh *mesh, const glm::vec3 &point, float radius)
    {
        std::vector<uint32_t> result;
        
        const auto &layout = mesh->GetVertexBufferLayout();
        auto stride = layout.GetStride();
        auto offset = layout["position"].offset;

        const auto &invMatrix = glm::inverse(mesh->GetEntity().lock()->GetWorldMatrix());
        auto pointLocalSpace = glm::vec3(invMatrix * glm::vec4(point, 1.f));

        auto vertBytes = (uint8_t *)mesh->GetVertices();
        for (size_t i = 0; i < mesh->GetVerticesLength(); i++)
        {
            auto v = *(glm::vec3 *)(vertBytes + i * stride + offset);

            if (glm::distance(pointLocalSpace, v) <= radius)
                result.push_back((uint32_t) i);
        }

        return result;
    }

    std::vector<uint32_t> GetVerticesInRadius(const Ref<SkeletalMesh> &mesh, const glm::vec3 &point, float radius)
    {
        return GetVerticesInRadius(mesh.get(), point, radius);
    }

    std::vector<uint32_t> GetVerticesInRadius(const Ref<StaticMesh> &mesh, const glm::vec3 &point, float radius)
    {
        return GetVerticesInRadius(mesh.get(), point, radius);
    }
}
