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
		Sound(const std::string& filename);
		~Sound();

		const std::string& get_filename();
 
		ma_decoder decoder; // todo: this shouldn't be public

	private:
		std::string filename;
	};
}

#endif