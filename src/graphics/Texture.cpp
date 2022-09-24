#define GLFW_INCLUDE_NONE

#include "Texture.h"

namespace xen
{
	Texture::Texture()
	{
		this->gl_texture_id = -1;
		this->height = 0;
		this->width = 0;
		this->mode = 0;
	}



	Texture::Texture(GLuint texture, int width, int height, int mode)
	{
		this->gl_texture_id = texture;
		this->width = width;
		this->height = height;
		this->mode = mode;
	}



	void Texture::unload() {
		
	}
	


	Texture::~Texture()
	{
		if (gl_texture_id > 0) {
			glDeleteTextures(1, &gl_texture_id);
		}
		//Advisor::logInfo("Deleted instance of Texture.");
	}
}