#pragma once

#include "Buffer.h"
#include "BufferLayout.h"

template<typename T>
class VertexBuffer;

class GenericVertexBuffer
{
public:
	GenericVertexBuffer(const BufferLayout &layout);
	virtual ~GenericVertexBuffer() = default;

public:
	virtual GLuint GetID() const = 0;

public:
	const BufferLayout layout;
};

template<typename T>
class VertexBuffer : public Buffer<T>, public GenericVertexBuffer
{
public:
	VertexBuffer(const BufferLayout &layout, GLenum usage = GL_STATIC_DRAW) : Buffer<T>(usage), GenericVertexBuffer(layout) {}
	virtual ~VertexBuffer() = default;

public:
	virtual GLuint GetID() const override { return GenericBuffer::GetID(); }
};
