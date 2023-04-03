#pragma once

#include "Core/Renderer/Buffer/BufferLayout.h"

struct Vertex
{
	Vertex();
	Vertex(const glm::vec3 &position);

	glm::vec3 position = glm::vec3(0.f);

	static const BufferLayout &layout;
};

struct StaticVertex : public Vertex
{
	StaticVertex();
	StaticVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec4 &tangent, const glm::vec2 &texCoord, const glm::vec3 &color);

	glm::vec3 normal = glm::vec3(0.f);
	glm::vec4 tangent = glm::vec4(0.f);
	glm::vec2 texCoord = glm::vec2(0.f);
	glm::vec3 color = glm::vec3(1.f);

	static const BufferLayout &layout;
};

struct SkeletalVertex : public StaticVertex
{
	SkeletalVertex();
	SkeletalVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec4 &tangent, const glm::vec2 &texCoord, const glm::vec3 &color, const glm::vec<4, uint16_t> &bones, const glm::vec4 &weights);

	glm::vec<4, uint16_t> bones = glm::vec<4, uint16_t>(0);
	glm::vec4 weights = glm::vec4(0.f);

	static const BufferLayout &layout;
};
