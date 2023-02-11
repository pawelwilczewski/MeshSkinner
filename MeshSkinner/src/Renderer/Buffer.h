#pragma once

#include "glad/glad.h"

template<typename T>
class Buffer
{
public:
	Buffer(GLenum type, GLenum usage = GL_DYNAMIC_DRAW) : type(type), usage(usage)
	{
		glCreateBuffers(1, &id);
	}

	virtual ~Buffer()
	{
		glDeleteBuffers(1, &id);
	}

public:
	void Bind() const
	{
		glBindBuffer(type, id);
	}

	void Unbind() const
	{
		glBindBuffer(type, 0);
	}

	void SetData(const T *data, GLuint length, GLuint lengthOffset = 0)
	{
		// automatically resize to twice the size if exceeded limit - like in the vector class etc.
		auto fullLength = lengthOffset + length;

		// don't do that if length is 0 - only just initialized
		if (this->length == 0)
		{
			this->capacity = fullLength;
			glNamedBufferData(id, this->capacity * sizeof T, nullptr, usage);
		}
		else if (fullLength > this->capacity)
		{
			// cache the buffer's data so we don't lose the data currently stored
			GLuint copiedBuffer;
			glCreateBuffers(1, &copiedBuffer);
			glNamedBufferData(copiedBuffer, this->length * sizeof T, nullptr, GL_STATIC_COPY);
			glCopyNamedBufferSubData(id, copiedBuffer, 0, 0, this->length * sizeof T);

			// perform the resizing
			this->capacity = fullLength * 2;
			glNamedBufferData(id, this->capacity * sizeof T, nullptr, usage);

			// restore the initial data
			glCopyNamedBufferSubData(copiedBuffer, id, 0, 0, this->length * sizeof T);

			// delete the temp buffer
			glDeleteBuffers(1, &copiedBuffer);
		}

		// append the data at the end - sufficient length ensured
		glNamedBufferSubData(id, lengthOffset * sizeof T, length * sizeof T, data);

		// update length if just extended
		if (fullLength > this->length)
			this->length = fullLength;
	}

	void Clear()
	{
		// no need to do unnecessary gl calls, just change the length
		this->length = 0;
	}

public:
	GLuint GetID() const { return id; }
	GLuint GetLength() const { return length; }
	GLuint GetCapacity() const { return capacity; }

protected:
	GLuint id;
	GLuint capacity = 0;
	GLuint length = 0;
	GLenum type;
	GLenum usage;
};
