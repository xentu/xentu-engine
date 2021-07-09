#ifndef XEN_SOUND_CPP
#define XEN_SOUND_CPP

#include <iostream>

#include "Sound.h"
#include "../utilities/Advisor.h"

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


	Sound::~Sound()
	{
		ma_decoder_uninit(&decoder);
		Advisor::logInfo("Deleted instance of Sound.");
	}
}

#endif