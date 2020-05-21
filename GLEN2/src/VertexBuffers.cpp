#include "VertexBuffers.h"

#include <numeric>
#include <stdexcept>

using namespace GLEN;

VertexBuffers::VertexBuffers(const size_t size)
	: 
	m_VBOs(size)
{
	std::iota(m_VBOs.begin(), m_VBOs.end(), 0);
}

void VertexBuffers::SetVBO(const int location)
{
	if (location < 0 || location >= m_VBOs.size())
	{
		throw std::out_of_range("VertexBuffers::SetVBO : index is out of range"); 
	}
}
