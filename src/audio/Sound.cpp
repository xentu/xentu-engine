#ifndef XEN_SOUND_CPP
#define XEN_SOUND_CPP

#include <iostream>

#include "Sound.h"

namespace xen
{
	Sound::Sound(std::string filename)
	{
		this->filename = filename;
		ma_result result;
		result = ma_decoder_init_file(filename.c_str(), NULL, &decoder);
		if (result != MA_SUCCESS) {
			throw result;
		}
	}



	void Sound::unload() {

	}



	Sound::~Sound()
	{
		ma_decoder_uninit(&decoder);
		std::cout << "Deleted instance of Sound." << std::endl;
	}
}

#endif