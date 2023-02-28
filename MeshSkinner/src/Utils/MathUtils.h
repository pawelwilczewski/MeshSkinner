#pragma once

#include "Application/Core.h"

struct Ray
{
	Ray(const glm::vec3 &origin = glm::vec3(0.f), const glm::vec3 &direction = glm::vec3(0.f));

	glm::vec3 origin;
	glm::vec3 direction;

public:
	bool IntersectsTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, glm::vec3 &intersectionPoint) const;
};
