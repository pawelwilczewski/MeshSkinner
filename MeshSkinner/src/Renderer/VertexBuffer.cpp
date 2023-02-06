#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const BufferLayout &layout, GLenum usage) : Buffer(GL_ARRAY_BUFFER, usage), layout(layout)
{

}
