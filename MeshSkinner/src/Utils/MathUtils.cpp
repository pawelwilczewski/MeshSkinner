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

        auto vertBytes = (uint8_t *)mesh->GetVertices();
        for (size_t i = 0; i < mesh->indices.size(); i += 3)
        {
            auto v0 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 0] * stride + offset);
            auto v1 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 1] * stride + offset);
            auto v2 = *(glm::vec3 *)(vertBytes + mesh->indices[i + 2] * stride + offset);

            v0 = glm::vec3(matrix * glm::vec4(v0, 1.f));
            v1 = glm::vec3(matrix * glm::vec4(v1, 1.f));
            v2 = glm::vec3(matrix * glm::vec4(v2, 1.f));

            // intersection check and update closest intersection
            glm::vec3 intersection;
            if (ray.IntersectsTriangle(v0, v1, v2, intersection))
            {
                auto distance = glm::distance(intersection, ray.origin);
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

    bool MathUtils::RayMeshIntersection(const Ray &ray, const Ref<SkeletalMesh> &mesh, glm::vec3 &intersection)
    {
        return RayMeshIntersection(ray, mesh.get(), intersection);
    }

    bool MathUtils::RayMeshIntersection(const Ray &ray, const Ref<StaticMesh> &mesh, glm::vec3 &intersection)
    {
        return RayMeshIntersection(ray, mesh.get(), intersection);
    }
}
