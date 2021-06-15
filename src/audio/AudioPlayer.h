#ifndef XEN_AUDIO_PLAYER_HPP
#define XEN_AUDIO_PLAYER_HPP

#include <string>

#include <miniaudio/miniaudio.h>
#include <luna/luna.hpp>
#include "Sound.h"

#define SAMPLE_FORMAT   ma_format_s16
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     44100
#define MIXER_ROWS      8

namespace xen
{
	/// <summary>
	/// A class responsible for playing audio via the MiniAudio API. An instance of
	/// this class is instansiated outside the game loop. The listen() method starts
	/// up the chosen audio device, close() stops it. play() is called during the
	/// game loop to play a sound.
	/// </summary>
	class AudioPlayer
	{
	public:
		AudioPlayer(lua_State* L);
		~AudioPlayer();


		/// <summary>Get weather a sound is playing.</summary>
		int lua_is_playing(lua_State* L);


		/// <summary>Play a sound!</summary>
		int lua_play(lua_State* L);


		/// <summary>Set the volume off a playing sound</summary>
		int lua_set_volume(lua_State* L);


		/// <summary>Stop playing a sound.</summary>
		int lua_stop(lua_State* L);


		//Class Constants
		static const char className[];


		// List of class properties that one can set/get from Lua
		static const Luna<AudioPlayer>::PropertyType properties[];


		// List of class methods to make available in Lua
		static const Luna<AudioPlayer>::FunctionType methods[];


	private:
		ma_device m_device;
		ma_device_config m_config;
		bool m_alive; // weather the class initialized ok.
		bool m_playing; // weather the device started ok.


		/// <summary>Get weather a sound is currently playing..</summary>
		bool is_playing(xen::Sound* sound);


		/// <summary>Spawn a playback instance of a sound, does not cancel previous playback, for that use _stop()</summary>
		int play(xen::Sound* sound);

		
		/// <summary>Stop playback for all instances of a specific sound.</summary>
		int stop(xen::Sound* sound);
	};
}

#endif