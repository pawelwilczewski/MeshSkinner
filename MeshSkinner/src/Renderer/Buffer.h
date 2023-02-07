#pragma once

#include "glad/glad.h"

template<typename T>
class Buffer
{
public:
	Buffer(GLenum type, GLenum usage = GL_STATIC_DRAW) : type(type), usage(usage)
	{
		glGenBuffers(1, &id);
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
			glBindBuffer(type, id);
			glBufferData(type, this->capacity * sizeof T, nullptr, usage);
		}
		else if (fullLength > this->capacity)
		{
			// cache the buffer's data so we don't lose the data currently stored
			auto copiedBuffer = CopyBuffer(id, this->length * sizeof T);

			// perform the resizing
			this->capacity = fullLength * 2;
			glBindBuffer(type, id);
			// TODO: should first delete the buffer before calling glBufferData?
			glBufferData(type, this->capacity * sizeof T, nullptr, usage);

			// restore the initial data
			glBindBuffer(GL_COPY_READ_BUFFER, copiedBuffer);
			glBindBuffer(GL_COPY_WRITE_BUFFER, id);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, this->length * sizeof T);			

			// delete the temp buffer
			glDeleteBuffers(1, &copiedBuffer);
		}

		// append the data at the end - sufficient length ensured
		glBufferSubData(type, lengthOffset * sizeof T, length * sizeof T, data);

		this->length = fullLength;
	}

private:
	GLuint CopyBuffer(GLuint srcBuffer, GLsizei size)
	{
		GLuint copyDest;
		glGenBuffers(1, &copyDest);
		glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer);
		glBindBuffer(GL_COPY_WRITE_BUFFER, copyDest);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, usage);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
		return copyDest;
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
