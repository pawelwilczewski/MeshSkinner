#pragma once

#include "Buffer.h"
#include "BufferLayout.h"

template<typename T>
class VertexBuffer : public Buffer<T>
{
public:
	VertexBuffer(const BufferLayout &layout, GLenum usage = GL_STATIC_DRAW) : Buffer<T>(GL_ARRAY_BUFFER, usage), layout(layout) {}
	virtual ~VertexBuffer() = default;

	const BufferLayout &GetLayout() const { return layout; }

private:
	const BufferLayout layout;
};
