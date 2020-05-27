#include "VertexBuffers.h"

#include <numeric>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace GLEN;

VertexBuffers::VertexBuffers(const size_t size)
	: 
	m_VBOs(size)
{
	std::iota(m_VBOs.begin(), m_VBOs.end(), 0);
	glGenBuffers(static_cast<int>(size), m_VBOs.data());
}

void VertexBuffers::SetVBO(const int location)
{
	if (location < 0 || location >= m_VBOs.size())
	{
		throw std::out_of_range("VertexBuffers::SetVBO : index is out of range"); 
	}


}
