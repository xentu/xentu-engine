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
			int StoreSound(Mix_Chunk* data);
			int StoreMusic(Mix_Music* data);
			void PlaySound(int id, int channel = -1, int loop = 0);
			void StopSound(int channel);
			void PlayMusic(int id, int loop = 0);
			void StopMusic();
			void SetSoundVolume(int id, float volume);
			void SetChannelVolume(int channel, float volume);
			void SetMusicVolume(float volume);
			void SetChannelPanning(int channel, float left, float right);
			int UnloadAudio(int id);
			int UnloadMusic(int id);

		private:
			bool m_initialized;
			int m_frequency;
			int m_channels;
			static AudioManager* m_instance;

			std::map<int, Mix_Chunk*> m_samples;
			int m_samples_iter = 0;

			std::map<int, Mix_Music*> m_music;
			int m_music_iter = 0;
			int m_playing_music_id = -1;
	};
}