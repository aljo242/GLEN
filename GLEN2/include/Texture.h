#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>


namespace GLEN{

struct TextureDescriptor
{
	GLenum internalFormat	= GL_RGB;
	GLint border			= 0;
	GLenum format			= GL_RGB;
	GLenum wrapS			= GL_REPEAT;
	GLenum wrapT			= GL_REPEAT;
	GLenum minFilter		= GL_LINEAR;
	GLenum maxFilter		= GL_LINEAR;
	GLenum type				= GL_UNSIGNED_BYTE;
}; // TextureDescriptor
	

class Texture
{
public:
	Texture(const std::filesystem::path path__, const TextureDescriptor& descriptor__);
	inline GLuint GetID() const noexcept {return texID;}
private:
	GLuint texID; 
	int width		{0};
	int height		{0};
	int numChannels {0};
	TextureDescriptor desc;

	

}; // Texture


} // namespace GLEN