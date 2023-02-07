#pragma once

#include "Application/Core.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

static GLenum ShaderTypeToGL(const ShaderDataType &type)
{
	switch (type)
	{
	case ShaderDataType::Float:		return GL_FLOAT;
	case ShaderDataType::Float2:	return GL_FLOAT;
	case ShaderDataType::Float3:	return GL_FLOAT;
	case ShaderDataType::Float4:	return GL_FLOAT;
	case ShaderDataType::Mat3:		return GL_FLOAT;
	case ShaderDataType::Mat4:		return GL_FLOAT;
	case ShaderDataType::Int:		return GL_INT;
	case ShaderDataType::Int2:		return GL_INT;
	case ShaderDataType::Int3:		return GL_INT;
	case ShaderDataType::Int4:		return GL_INT;
	case ShaderDataType::Short:		return GL_SHORT;
	case ShaderDataType::Short2:	return GL_SHORT;
	case ShaderDataType::Short3:	return GL_SHORT;
	case ShaderDataType::Short4:	return GL_SHORT;
	case ShaderDataType::Bool:		return GL_BOOL;
	default:						assert(false); return 0;
	}
}

template<typename V, typename I>
class VertexArray
{
public:
	VertexArray()
	{
		glGenVertexArrays(1, &id);
	}
	virtual ~VertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}

public:
	void Bind() const
	{
		glBindVertexArray(id);
	}

	void Unbind() const
	{
		glBindVertexArray(0);
	}

	void SetVertexBuffer(Ref<VertexBuffer<V>> vertexBuffer)
	{
		glBindVertexArray(id);
		//vertexBuffer->Bind();

		assert(vertexBuffer->layout.GetElements().size() > 0);

		uint32_t index = 0;
		for (const auto &element : vertexBuffer->layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderTypeToGL(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->layout.GetStride(),
				(const void *)element.offset
			);
			index++;
		}

		this->vertexBuffer = vertexBuffer;
	}

	void SetIndexBuffer(Ref<IndexBuffer<I>> indexBuffer)
	{
		//indexBuffer->Bind();
		glVertexArrayElementBuffer(id, indexBuffer->GetID());
		this->indexBuffer = indexBuffer;
	}

public:
	

private:
	GLuint id;
	Ref<VertexBuffer<V>> vertexBuffer;
	Ref<IndexBuffer<I>> indexBuffer;
};
