#include "BufferLayout.h"
#include "Log.h"
#include <numeric>

using namespace GLEN;

constexpr int VBstrideSize {8};


VertexBufferLayout::VertexBufferLayout(const std::vector<GLint> entrySizes__, const GLenum type__)
	:
	entrySizes(entrySizes__),
	type(type__),
	totalEntries(std::accumulate(entrySizes__.begin(), entrySizes__.end(), 0ul)),
	dataSize(sizeof(GLfloat))
{
	if (type == GL_UNSIGNED_INT)
	{
		dataSize = sizeof(GLuint);
	}
}

// TODO only handling pre-normalized data, need to handle non-normalized
void VertexBufferLayout::SetVertexAttrib(const GLuint index) const
{
	GLEN_ERROR("Binding: index: {}, size: {}, stride: {}, pointer: {}", index, entrySizes[index], totalEntries * dataSize, GetPointer(index));
	glVertexAttribPointer(index, entrySizes[index], type, GL_FALSE, totalEntries * dataSize, GetPointer(index));
}


const void* VertexBufferLayout::GetPointer(const size_t index) const
{
	size_t entrySize {0ull}; // offset is 0 if first entry, otherwise is the size of the previous
	if (index > 0ull)
	{
		
		entrySize = std::accumulate(entrySizes.begin(), entrySizes.begin() + index, 0ull);
	}

	return (void*)(entrySize * dataSize);
}