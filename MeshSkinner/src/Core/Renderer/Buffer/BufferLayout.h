#pragma once

#include "Application/Core.h"

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Short, Short2, Short3, Short4, UnsignedShort, UnsignedShort2, UnsignedShort3, UnsignedShort4, Bool
};

class BufferElement
{
public:
	BufferElement(const std::string &name, ShaderDataType type, bool normalized = false);
	virtual ~BufferElement() = default;

public:
	uint32_t GetComponentCount() const;

public:
	std::string name;
	ShaderDataType type;
	bool normalized;
	uint32_t size;
	uint32_t offset;
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<BufferElement> &elements);
	virtual ~BufferLayout() = default;

public:
	const std::vector<BufferElement> &GetElements() const;
	uint32_t GetStride() const;

	std::vector<BufferElement>::iterator begin();
	std::vector<BufferElement>::iterator end();
	std::vector<BufferElement>::const_iterator begin() const;
	std::vector<BufferElement>::const_iterator end() const;

private:
	std::vector<BufferElement> elements;
	uint32_t stride;
};
