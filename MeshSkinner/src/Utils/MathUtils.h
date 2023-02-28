#pragma once

#include "Application/Core.h"

struct Ray
{
	Ray(const glm::vec3 &origin = glm::vec3(0.f), const glm::vec3 &direction = glm::vec3(0.f));

	glm::vec3 origin;
	glm::vec3 direction;
};
