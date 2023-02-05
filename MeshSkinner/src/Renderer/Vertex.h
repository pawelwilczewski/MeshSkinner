#pragma once

#include "Application/Core.h"
#include "BufferLayout.h"

class Vertex
{
public:
	Vertex(const glm::vec3 &position) : position(position) {}
	virtual ~Vertex() = default;

public:
	glm::vec3 position = glm::vec3(0.f);

	inline static BufferLayout &Layout = BufferLayout({
		{ "position", ShaderDataType::Float3 }
		});
};

class StaticVertex : public Vertex
{
public:
	StaticVertex(
		const glm::vec3 &position,
		const glm::vec2 &texCoord,
		const glm::vec3 &normal,
		const glm::vec3 &color) :
		Vertex(position),
		texCoord(texCoord),
		normal(normal),
		color(color) {}

	virtual ~StaticVertex() = default;

public:
	glm::vec2 texCoord = glm::vec2(0.f);
	glm::vec3 normal = glm::vec3(0.f);
	glm::vec3 color = glm::vec3(1.f);

	inline static BufferLayout &Layout = BufferLayout({
		{ "position", ShaderDataType::Float3 },
		{ "texCoord", ShaderDataType::Float2 },
		{ "normal", ShaderDataType::Float3 },
		{ "color", ShaderDataType::Float3 }
		});
};

class SkeletalVertex : public StaticVertex
{
public:
	SkeletalVertex(
		const glm::vec3 &position,
		const glm::vec2 &texCoord,
		const glm::vec3 &normal,
		const glm::vec3 &color,
		const glm::vec<4, uint16_t> &bones,
		const glm::vec4 &weights) :
		StaticVertex(position, texCoord, normal, color),
		bones(bones),
		weights(weights) {}

	virtual ~SkeletalVertex() = default;

public:
	glm::vec<4, uint16_t> bones;
	glm::vec4 weights;

	inline static BufferLayout &Layout = BufferLayout({
		{ "position", ShaderDataType::Float3 },
		{ "texCoord", ShaderDataType::Float2 },
		{ "normal", ShaderDataType::Float3 },
		{ "color", ShaderDataType::Float3 },
		{ "bones", ShaderDataType::Short4 },
		{ "weights", ShaderDataType::Float4 },
		});
};
