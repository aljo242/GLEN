#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>


namespace GLEN{

class Texture
{
public:
	Texture(const std::filesystem::path path__);
	inline GLuint GetID() const noexcept {return texID;}
private:
	GLuint texID; 
	int width		{0};
	int height		{0};
	int numChannels {0};

};


} // namespace GLEN