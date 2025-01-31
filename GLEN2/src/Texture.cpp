#include "Texture.h"
#include "Log.h"
#include "stb_image.h"


using namespace GLEN;
// TODO possibly refactor to hold more than 1 textures
// maybe create a texture buffer that accepts a list of paths
// also need to add some kind of struct that is a texture descriptor 
// struct will include WRAP_S, WRAP_T and MIN, MAG stuff


Texture::Texture(const std::filesystem::path path__, const TextureDescriptor& descriptor__)
	: 
	desc(descriptor__)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapS); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapT); // map to:       x, y, z

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.maxFilter); // map to:       x, y, z

	unsigned char* data {stbi_load(path__.string().c_str(), &width, &height, &numChannels, 0)};

	if (data)
	{
		// TODO handle "level" param
		glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFormat, 
			width, height, desc.border, desc.format, desc.type, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		GLEN_CRITICAL("ERROR::TEXTURE::FAILED_LOAD");
	}
	stbi_image_free(data);
}