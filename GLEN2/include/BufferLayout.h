#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace GLEN{


	// TODO remove the pointers attrib.  We can use the previous size + previous pointer to eliminate
class VertexBufferLayout
{
public:
	VertexBufferLayout(const std::vector<GLint> entrySizes__, const GLenum type__); // init and add first entry
	void SetVertexAttrib(const GLuint index) const;
	inline GLint GetEntrySize(const GLuint index) const {return entrySizes[index];} // TODO add checking, possibly remove inline
	inline GLuint GetTotalEntries() const noexcept {return totalEntries;}

private:
	// field 
	std::vector<GLint>		entrySizes;
	GLenum					type;
	GLuint					totalEntries;
	GLsizei					dataSize;

	const void* GetPointer(const size_t index) const;
};

} // namespace GLEN