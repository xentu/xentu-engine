#pragma once

#include <map>
#include <SDL_mixer.h>

#include "../Config.h"

namespace xen
{
	class AudioManager
	{
		public:
			AudioManager(Config* config);
			~AudioManager();
			static AudioManager* GetInstance();
			int StoreSample(Mix_Chunk* data);
			int StoreMusic(Mix_Music* data);

		private:
			bool m_initialized;
			int m_frequency;
			int m_channels;
			static AudioManager* m_instance;

			std::map<int, Mix_Chunk*> m_samples;
			int m_samples_iter = 0;

			std::map<int, Mix_Music*> m_music;
			int m_music_iter = 0;
	};
}