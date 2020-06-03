#pragma once

#include "BufferLayout.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLEN {

template <typename dataType = GLfloat, auto enumType = GL_FLOAT>
class VertexBuffer
{
public:
	VertexBuffer(const VertexBufferLayout& layout__, const std::vector<dataType>& buffer__)
		:
		layout(layout__), // use default copy constructor
		buffer(buffer__)
	{}

	VertexBuffer(const std::vector<GLint> entrySizes__, const std::vector<dataType>& buffer__)
		:
		layout(entrySizes__, enumType),
		buffer(buffer__)
	{}

	~VertexBuffer()
	{
		glDeleteVertexArrays(numVAOs, &VAO);
		glDeleteBuffers(numVBOs, &VBO);
	}

	void GenBuffers(const GLsizei numBufs) 
	{
		glGenBuffers(numBufs, &VBO);
		numVBOs = numBufs;
	}
	void GenArrays(const GLsizei numBufs) 
	{
		glGenVertexArrays(numBufs, &VAO);
		numVAOs = numBufs;
	}
	inline void Bind() const {glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER, VBO);}
	inline void BufferStatic() const {glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(dataType), buffer.data(), GL_STATIC_DRAW);}

	void PushVert(const GLuint index) const // TODO maybe move attribute creation to construction
	{  
		layout.SetVertexAttrib(index);
		glEnableVertexAttribArray(index);
	}

	inline void BindArray() const {glBindVertexArray(VAO);}

private:

	VertexBufferLayout layout;
	std::vector<dataType> buffer;
	GLuint VBO;
	GLuint VAO;
	GLsizei numVBOs;
	GLsizei numVAOs;
};


} // namespace GLEN