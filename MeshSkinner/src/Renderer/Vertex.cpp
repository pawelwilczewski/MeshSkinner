#include "pch.h"
#include "Vertex.h"

const BufferLayout &Vertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 }
});

const BufferLayout &StaticVertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 },
	{ "texCoord", ShaderDataType::Float2 },
	{ "normal", ShaderDataType::Float3 },
	{ "color", ShaderDataType::Float3 }
});

const BufferLayout &SkeletalVertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 },
	{ "texCoord", ShaderDataType::Float2 },
	{ "normal", ShaderDataType::Float3 },
	{ "color", ShaderDataType::Float3 },
	{ "bones", ShaderDataType::Short4 },
	{ "weights", ShaderDataType::Float4 },
});

Vertex::Vertex(const glm::vec3 &position) : position(position)
{

}

StaticVertex::StaticVertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal, const glm::vec3 &color) : Vertex(position), texCoord(texCoord), normal(normal), color(color)
{

}

SkeletalVertex::SkeletalVertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal, const glm::vec3 &color, const glm::vec<4, uint16_t> &bones, const glm::vec4 &weights) : StaticVertex(position, texCoord, normal, color), bones(bones), weights(weights)
{

}
