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
    // get verts as calculated on the gpu
    auto &info = Renderer::skeletalMeshDrawCalls.at(mesh->material->shader);
    auto length = mesh->GetVerticesLength();
    auto verts = std::make_unique<glm::vec4[]>(length);
    info->finalPos->ReadData(info->meshes.at(mesh), length, verts.get());

    float smallestDistance = -1.f;

    for (size_t i = 0; i < mesh->indices.size(); i += 3)
    {
        const auto &v0 = verts[mesh->indices[i + 0]];
        const auto &v1 = verts[mesh->indices[i + 1]];
        const auto &v2 = verts[mesh->indices[i + 2]];

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
    // get verts as calculated on the gpu
    auto &info = Renderer::skeletalMeshDrawCalls.at(mesh->material->shader);
    auto length = mesh->GetVerticesLength();
    auto verts = std::make_unique<glm::vec4[]>(length);
    info->finalPos->ReadData(info->meshes.at(mesh), length, verts.get());

    std::vector<uint32_t> result;

    for (size_t i = 0; i < mesh->GetVerticesLength(); i++)
    {
        if (glm::distance(point, glm::vec3(verts[i])) <= radius)
            result.push_back((uint32_t)i);
    }

    return result;
}
