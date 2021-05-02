#ifndef XEN_TEXTURE_CPP
#define XEN_TEXTURE_CPP

#include <GLEW/GL/glew.h>
#include <iostream>

#include "Texture.hpp"

namespace xen
{
	Texture::Texture()
	{
		this->gl_texture_id = -1;
		this->height = 0;
		this->width = 0;
		this->channels = 0;
	}



	Texture::Texture(unsigned int texture, int width, int height, int channels)
	{
		this->gl_texture_id = texture;
		this->width = width;
		this->height = height;
		this->channels = channels;
	}



	void Texture::unload() {
		
	}
	


	Texture::~Texture()
	{
		if (gl_texture_id > 0) {
			glDeleteTextures(1, &gl_texture_id);
		}
		std::cout << "Deleted instance of Texture." << std::endl;
	}
}

#endif