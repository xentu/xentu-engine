#ifndef XEN_TEXTURE_HPP
#define XEN_TEXTURE_HPP

#include <map>
#include <string>

namespace xen
{
	class Texture
	{
	public:
		Texture();
		Texture(unsigned int texture, int width, int height, int channels);
		~Texture();

		void unload();

		unsigned int gl_texture_id;
		int width;
		int height;
		int channels;
	};
}

#endif