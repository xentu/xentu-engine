#ifndef XEN_AUDIO_PLAYER_HPP
#define XEN_AUDIO_PLAYER_HPP

#include <string>

#include <miniaudio/miniaudio.h>
#include <luna/luna.hpp>
#include "Sound.h"

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
	private:
		//AudioPlayerState* m_state;
		ma_device m_device;
		ma_device_config m_config;
		bool m_alive; // weather the class initialized ok.
		bool m_playing; // weather the device started ok.

		/// <summary>
		/// Callback used to test the audio playback using miniaudio.
		/// </summary>
		static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
		static ma_uint16 read_and_mix_pcm_frames_f16(ma_decoder* pDecoder, ma_int16* pOutputF16, ma_uint16 frameCount);
	public:
		AudioPlayer(lua_State* L);
		~AudioPlayer();

		/// <summary>
		/// Play a sound!
		/// </summary>
		int play(lua_State* L);

		/// <summary>
		/// Play a sound!
		/// </summary>
		/// <param name="sound">The sound instance to play.</param>
		int __play(Sound* sound);

		/// <summary>
		/// Get weather this audio player is alive (device started successfully and can play).
		/// </summary>
		bool alive();

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<AudioPlayer>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<AudioPlayer>::FunctionType methods[];

	
	};
}

#endif