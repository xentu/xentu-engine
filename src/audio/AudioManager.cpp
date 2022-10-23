#include "AudioManager.h"
#include "../Globals.h"

namespace xen
{
	AudioManager::AudioManager(Config* config)
	{
		// keep a pointer to this instance.
		if (m_instance != nullptr) {
			XEN_ERROR("> Error, tried to create more than one AssetManager!");
			exit(111);
		}

		auto opts = config->audio;
		Mix_Init(MIX_INIT_OGG);
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
			XEN_ERROR("Error, called AudioManager::GetInstance() before initialization!");
			exit(123);
		}
		return m_instance;
	}


	int AudioManager::StoreSound(Mix_Chunk* data)
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


	void AudioManager::PlaySound(int id, int channel, int loop)
	{
		Mix_PlayChannel(channel, m_samples[id], 0);
	}


	void AudioManager::StopSound(int channel)
	{
		Mix_HaltChannel(-1);
	}


	void AudioManager::PlayMusic(int id, int loop)
	{
		auto music = m_music[id];

		// check to see if other music is playing, and stop it if that's the case.
		if (m_playing_music_id != id && (Mix_PlayingMusic() == 1 || Mix_PausedMusic() == 1))
		{
			Mix_HaltMusic();
		}
		m_playing_music_id = id;

		// if music is not playing, play it.
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(music, loop);
		}
		else
		{
			if(Mix_PausedMusic() == 1)
         {
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
		}
	}
	

	void AudioManager::StopMusic()
	{
		if (Mix_PlayingMusic() == 1 || Mix_PausedMusic() == 1)
		{
			Mix_HaltMusic();
		}
	}


	void AudioManager::SetSoundVolume(int id, float volume)
	{
		int i_vol = static_cast<int>(127 * volume);
		Mix_VolumeChunk(m_samples[id], i_vol);
	}


	void AudioManager::SetChannelVolume(int channel, float volume)
	{
		int i_vol = static_cast<int>(127 * volume);
		Mix_Volume(channel, i_vol);
	}
	
	
	void AudioManager::SetMusicVolume(float volume)
	{
		int i_vol = static_cast<int>(127 * volume);
		Mix_VolumeMusic(i_vol);
	}
	
	
	void AudioManager::SetChannelPanning(int channel, float left, float right)
	{
		int i_left = static_cast<int>(255 * left);
		int i_right = static_cast<int>(255 * right);
		Mix_SetPanning(channel, i_left, i_right);
	}
}