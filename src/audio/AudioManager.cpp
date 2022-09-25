#include "AudioManager.h"
#include "../Globals.h"

namespace xen
{
	AudioManager::AudioManager(Config* config)
	{
		// keep a pointer to this instance.
		if (m_instance != nullptr) {
			printf("Error, tried to create more than one AssetManager!");
			exit(111);
		}

		auto opts = config->audio;
		if(Mix_OpenAudio(opts.frequency, MIX_DEFAULT_FORMAT, opts.channels, 2048) < 0)
		{
			XEN_ERROR( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
      }
		else
		{
			m_initialized = true;
			m_instance = this;
		}
	}

	AudioManager::~AudioManager()
	{
		for (auto const& s : m_samples)
		{
			Mix_FreeChunk(s.second);
		}
		m_samples.clear();

		for (auto const& m : m_music)
		{
			Mix_FreeMusic(m.second);
		}
		m_music.clear();
		Mix_Quit();
		m_instance = NULL;
	}


	AudioManager* AudioManager::m_instance = 0;
	AudioManager* AudioManager::GetInstance()
	{
		if (m_instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return m_instance;
	}


	int AudioManager::StoreSample(Mix_Chunk* data)
	{
		m_samples.insert(std::make_pair(m_samples_iter, data));
		m_samples_iter++;
		return m_samples_iter - 1;
	}


	int AudioManager::StoreMusic(Mix_Music* data)
	{
		m_music.insert(std::make_pair(m_music_iter, data));
		m_music_iter++;
		return m_music_iter - 1;
	}
}