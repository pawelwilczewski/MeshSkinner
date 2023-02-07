#include "pch.h"
#include "BufferLayout.h"

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
	default:						assert(false); return 0;
	}
}

BufferElement::BufferElement(const std::string &name, ShaderDataType type, bool normalized) : name(name), type(type), normalized(normalized), size(GetShaderDataTypeSize(type)), offset(0)
{

}

uint32_t BufferElement::GetComponentCount() const
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
	default:						assert(false); return 0;
	}
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &elements) : elements(elements)
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

const std::vector<BufferElement> &BufferLayout::GetElements() const { return elements; }
uint32_t BufferLayout::GetStride() const { return stride; }

std::vector<BufferElement>::iterator BufferLayout::begin() { return elements.begin(); }
std::vector<BufferElement>::iterator BufferLayout::end() { return elements.end(); }
std::vector<BufferElement>::const_iterator BufferLayout::begin() const { return elements.begin(); }
std::vector<BufferElement>::const_iterator BufferLayout::end() const { return elements.end(); }
