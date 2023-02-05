#pragma once

#include "Application/Core.h"

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Short, Short2, Short3, Short4, Bool
};

static uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:		return 4;
	case ShaderDataType::Float2:	return 4 * 2;
	case ShaderDataType::Float3:	return 4 * 3;
	case ShaderDataType::Float4:	return 4 * 4;
	case ShaderDataType::Mat3:		return 4 * 3 * 3;
	case ShaderDataType::Mat4:		return 4 * 4 * 4;
	case ShaderDataType::Int:		return 4;
	case ShaderDataType::Int2:		return 4 * 2;
	case ShaderDataType::Int3:		return 4 * 3;
	case ShaderDataType::Int4:		return 4 * 4;
	case ShaderDataType::Short:		return 2;
	case ShaderDataType::Short2:	return 2 * 2;
	case ShaderDataType::Short3:	return 2 * 3;
	case ShaderDataType::Short4:	return 2 * 4;
	case ShaderDataType::Bool:		return 1;
	default:						assert(false);
	}
}

class BufferElement
{
public:
	BufferElement(const std::string &name, ShaderDataType type)
		: name(name), type(type), size(GetShaderDataTypeSize(type)), offset(0)
	{}

	uint32_t GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Short:		return 1;
		case ShaderDataType::Short2:	return 2;
		case ShaderDataType::Short3:	return 3;
		case ShaderDataType::Short4:	return 4;
		case ShaderDataType::Bool:		return 1;
		default:						assert(false);
		}
	}

public:
	std::string name;
	ShaderDataType type;
	uint32_t size;
	uint32_t offset;
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<BufferElement> &elements) : elements(elements)
	{
		uint32_t offset = 0;
		stride = 0;
		for (auto &element : this->elements)
		{
			element.offset = offset;
			offset += element.size;
			stride += element.size;
		}
	}

	const std::vector<BufferElement> &GetElements() const { return elements; }
	uint32_t GetStride() const { return stride; }

	std::vector<BufferElement>::iterator begin() { return elements.begin(); }
	std::vector<BufferElement>::iterator end() { return elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

private:
	std::vector<BufferElement> elements;
	uint32_t stride;
};
