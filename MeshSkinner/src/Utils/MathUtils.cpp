#include "pch.h"
#include "MathUtils.h"

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
