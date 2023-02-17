#include "pch.h"
#include "Vertex.h"

const BufferLayout &Vertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 }
});

const BufferLayout &StaticVertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 },
	{ "normal", ShaderDataType::Float3 },
	{ "tangent", ShaderDataType::Float4 },
	{ "texCoord", ShaderDataType::Float2 },
	{ "color", ShaderDataType::Float3 }
});

const BufferLayout &SkeletalVertex::layout = BufferLayout({
	{ "position", ShaderDataType::Float3 },
	{ "normal", ShaderDataType::Float3 },
	{ "tangent", ShaderDataType::Float4 },
	{ "texCoord", ShaderDataType::Float2 },
	{ "color", ShaderDataType::Float3 },
	{ "bones", ShaderDataType::Short4 },
	{ "weights", ShaderDataType::Float4 }
});

Vertex::Vertex()
{

}

Vertex::Vertex(const glm::vec3 &position) : position(position)
{

}

StaticVertex::StaticVertex() : Vertex()
{

}

StaticVertex::StaticVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec4 &tangent, const glm::vec2 &texCoord, const glm::vec3 &color) : Vertex(position), normal(normal), tangent(tangent), texCoord(texCoord), color(color)
{

}

SkeletalVertex::SkeletalVertex() : StaticVertex()
{

}

SkeletalVertex::SkeletalVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec4 &tangent, const glm::vec2 &texCoord, const glm::vec3 &color, const glm::vec<4, uint16_t> &bones, const glm::vec4 &weights) : StaticVertex(position, normal, tangent, texCoord, color), bones(bones), weights(weights)
{

}
