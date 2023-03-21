#pragma once

#include "Application/Core.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"

static GLenum ShaderTypeToGL(const ShaderDataType &type)
{
	switch (type)
	{
	case ShaderDataType::Float:				return GL_FLOAT;
	case ShaderDataType::Float2:			return GL_FLOAT;
	case ShaderDataType::Float3:			return GL_FLOAT;
	case ShaderDataType::Float4:			return GL_FLOAT;
	case ShaderDataType::Mat3:				return GL_FLOAT;
	case ShaderDataType::Mat4:				return GL_FLOAT;
	case ShaderDataType::Int:				return GL_INT;
	case ShaderDataType::Int2:				return GL_INT;
	case ShaderDataType::Int3:				return GL_INT;
	case ShaderDataType::Int4:				return GL_INT;
	case ShaderDataType::UnsignedInt:		return GL_UNSIGNED_INT;
	case ShaderDataType::UnsignedInt2:		return GL_UNSIGNED_INT;
	case ShaderDataType::UnsignedInt3:		return GL_UNSIGNED_INT;
	case ShaderDataType::UnsignedInt4:		return GL_UNSIGNED_INT;
	case ShaderDataType::Short:				return GL_SHORT;
	case ShaderDataType::Short2:			return GL_SHORT;
	case ShaderDataType::Short3:			return GL_SHORT;
	case ShaderDataType::Short4:			return GL_SHORT;
	case ShaderDataType::UnsignedShort:		return GL_UNSIGNED_SHORT;
	case ShaderDataType::UnsignedShort2:	return GL_UNSIGNED_SHORT;
	case ShaderDataType::UnsignedShort3:	return GL_UNSIGNED_SHORT;
	case ShaderDataType::UnsignedShort4:	return GL_UNSIGNED_SHORT;
	case ShaderDataType::Bool:				return GL_BOOL;
	default:								assert(false); return 0;
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

	void SetVertexBuffer(Ref<GenericVertexBuffer> vertexBuffer, size_t bindingIndex)
	{
		if (bindingIndex + 1 > vertexBuffers.size())
			vertexBuffers.resize(bindingIndex + 1);

		// add to the vertex buffers vector
		vertexBuffers[bindingIndex] = vertexBuffer;

		glVertexArrayVertexBuffer(id, bindingIndex, vertexBuffer->GetID(), 0, vertexBuffer->layout.GetStride());

		assert(vertexBuffer->layout.GetElements().size() > 0);

		for (const auto &element : vertexBuffer->layout)
		{
			glEnableVertexArrayAttrib(id, attributeIndex);

			if (
				element.type == ShaderDataType::Float ||
				element.type == ShaderDataType::Float2 ||
				element.type == ShaderDataType::Float3 ||
				element.type == ShaderDataType::Float4 ||
				element.type == ShaderDataType::Mat3 ||
				element.type == ShaderDataType::Mat4
				)
				glVertexArrayAttribFormat(
					id,
					attributeIndex,
					element.GetComponentCount(),
					ShaderTypeToGL(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					element.offset
				);
			else if (
				element.type == ShaderDataType::Int ||
				element.type == ShaderDataType::Int2 ||
				element.type == ShaderDataType::Int3 ||
				element.type == ShaderDataType::Int4 ||
				element.type == ShaderDataType::UnsignedInt ||
				element.type == ShaderDataType::UnsignedInt2 ||
				element.type == ShaderDataType::UnsignedInt3 ||
				element.type == ShaderDataType::UnsignedInt4 ||
				element.type == ShaderDataType::Short ||
				element.type == ShaderDataType::Short2 ||
				element.type == ShaderDataType::Short3 ||
				element.type == ShaderDataType::Short4 ||
				element.type == ShaderDataType::UnsignedShort ||
				element.type == ShaderDataType::UnsignedShort2 ||
				element.type == ShaderDataType::UnsignedShort3 ||
				element.type == ShaderDataType::UnsignedShort4 ||
				element.type == ShaderDataType::Bool
				)
				glVertexArrayAttribIFormat(
					id,
					attributeIndex,
					element.GetComponentCount(),
					ShaderTypeToGL(element.type),	
					element.offset
				);
			else
				assert(false);

			glVertexArrayAttribBinding(id, attributeIndex, bindingIndex);
			attributeIndex++;
		}

		vboBindingIndex = bindingIndex + 1;
	}

	uint16_t AddVertexBuffer(Ref<GenericVertexBuffer> vertexBuffer)
	{
		SetVertexBuffer(vertexBuffer, vboBindingIndex);
		return vboBindingIndex++;
	}

	void SetIndexBuffer(Ref<IndexBuffer<I>> indexBuffer)
	{
		glVertexArrayElementBuffer(id, indexBuffer->GetID());

		this->indexBuffer = indexBuffer;
	}

	// TODO: VertexBuffer shouldn't be templated because we might want to add different type vertex buffers to this vao
	const Ref<GenericVertexBuffer> &GetVertexBuffer(uint16_t bindingIndex) const { return vertexBuffers[bindingIndex]; }
	const Ref<IndexBuffer<I>> &GetIndexBuffer() const { return indexBuffer; }

private:
	GLuint id;
	uint16_t vboBindingIndex = 0;
	uint16_t attributeIndex = 0;
	std::vector<Ref<GenericVertexBuffer>> vertexBuffers;
	Ref<IndexBuffer<I>> indexBuffer;
};
