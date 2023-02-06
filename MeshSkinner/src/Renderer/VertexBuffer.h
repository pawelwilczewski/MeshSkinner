#pragma once

#include "Buffer.h"
#include "BufferLayout.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const BufferLayout &layout, GLenum usage = GL_STATIC_DRAW);
	virtual ~VertexBuffer() = default;

private:
	const BufferLayout layout;
};
