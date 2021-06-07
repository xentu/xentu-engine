#ifndef XEN_SOUND_HPP
#define XEN_SOUND_HPP

#include <map>
#include <string>

#include <miniaudio/miniaudio.h>

namespace xen
{
	class Sound
	{
	public:
		Sound(std::string filename);
		~Sound();

		void unload();

		std::string filename;
		ma_decoder decoder;
	};
}

#endif