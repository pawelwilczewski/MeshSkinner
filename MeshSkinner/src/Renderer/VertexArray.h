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

template<typename I>
class VertexArray
{
public:
	VertexArray()
	{
		glCreateVertexArrays(1, &id);
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

	void SetVertexBuffer(Ref<GenericVertexBuffer> vertexBuffer, uint16_t bindingIndex)
	{
		glVertexArrayVertexBuffer(id, bindingIndex, vertexBuffer->GetID(), 0, vertexBuffer->layout.GetStride());

		// add to the vertex buffers map
		if (vertexBuffers.find(bindingIndex) == vertexBuffers.end())
			vertexBuffers.insert({ bindingIndex, vertexBuffer });
		else
			vertexBuffers[bindingIndex] = vertexBuffer;

		assert(vertexBuffer->layout.GetElements().size() > 0);

		uint32_t index = 0;
		for (const auto &element : vertexBuffer->layout)
		{
			glEnableVertexArrayAttrib(id, index);
			glVertexArrayAttribFormat(id, index, element.GetComponentCount(), ShaderTypeToGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, element.offset);
			glVertexArrayAttribBinding(id, index, bindingIndex);
			index++;
		}
	}

	// TODO: implement
	//uint16_t AddVertexBuffer(Ref<GenericVertexBuffer> vertexBuffer)
	//{
	//	return bindingIndex;
	//}

	void SetIndexBuffer(Ref<IndexBuffer<I>> indexBuffer)
	{
		glVertexArrayElementBuffer(id, indexBuffer->GetID());

		this->indexBuffer = indexBuffer;
	}

	// TODO: VertexBuffer shouldn't be templated because we might want to add different type vertex buffers to this vao
	const Ref<GenericVertexBuffer> &GetVertexBuffer(uint16_t bindingIndex) const { return vertexBuffers.at(bindingIndex); }
	const Ref<IndexBuffer<I>> &GetIndexBuffer() const { return indexBuffer; }

private:
	GLuint id;
	uint16_t bindingIndex = -1;
	std::unordered_map<uint16_t, Ref<GenericVertexBuffer>> vertexBuffers;
	Ref<IndexBuffer<I>> indexBuffer;
};
