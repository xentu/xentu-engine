#pragma once

#include <GL/glew.h>
#include <iostream>
#include <map>
#include <string>

namespace xen
{
	class Texture
	{
	public:
		Texture();
		Texture(GLuint texture, int width, int height, int mode);
		~Texture();

		void unload();

		GLuint gl_texture_id;
		int width;
		int height;
		int mode;
	};
}