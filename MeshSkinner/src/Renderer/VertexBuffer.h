#pragma once

#include "Buffer.h"
#include "BufferLayout.h"

template<typename T>
class VertexBuffer : public Buffer<T>
{
public:
	// TODO: URGENT: SHOULD layout be a part of VertexArray instead???
	VertexBuffer(const BufferLayout &layout, GLenum usage = GL_DYNAMIC_DRAW) : Buffer<T>(GL_ARRAY_BUFFER, usage), layout(layout) {}
	virtual ~VertexBuffer() = default;

public:
	const BufferLayout layout;
};
