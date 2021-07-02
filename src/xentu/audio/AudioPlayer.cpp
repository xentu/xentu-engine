#ifndef XEN_AUDIO_PLAYER_CPP
#define XEN_AUDIO_PLAYER_CPP

#define MINIAUDIO_IMPLEMENTATION

#include <iostream>
#include <thread>
#include <chrono>

#include "../XentuGame.h"
#include "AudioPlayer.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	#pragma region MiniAudio Helpers


	#define MIXER_STOPPED 0
	#define MIXER_PLAYING 1


	struct xen_decoder_opts { int state; float volume; };


	xen_decoder_opts m_decoder_opts[MIXER_ROWS]; // array of decoder info
	ma_decoder* m_decoder_refs[MIXER_ROWS];
	//ma_bool32* m_decoders_at_end;
	ma_data_converter_config m_converter_config;
	ma_data_converter m_converter;


	ma_uint16 _read_and_mix_pcm_frames_f16(ma_decoder* pDecoder, ma_int16* pOutputF16, ma_uint16 frameCount, float volume)
	{
		/*
		The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
		contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
		doing that in this example.
		*/
		ma_int16 temp[4096];
		ma_int16 tempCapInFrames = ma_countof(temp) / CHANNEL_COUNT;
		ma_int16 totalFramesRead = 0;

		while (totalFramesRead < frameCount) {
			ma_int16 iSample;
			ma_int16 framesReadThisIteration;
			ma_int16 totalFramesRemaining = frameCount - totalFramesRead;
			ma_int16 framesToReadThisIteration = tempCapInFrames;
			if (framesToReadThisIteration > totalFramesRemaining) {
				framesToReadThisIteration = totalFramesRemaining;
			}

			framesReadThisIteration = (ma_int16)ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration);
			if (framesReadThisIteration == 0) {
				break;
			}

			/* Mix the frames together. */
			for (iSample = 0; iSample < framesReadThisIteration * CHANNEL_COUNT; ++iSample) {
				pOutputF16[totalFramesRead * CHANNEL_COUNT + iSample] += (int)(temp[iSample] * volume);
			}

			totalFramesRead += framesReadThisIteration;

			if (framesReadThisIteration < framesToReadThisIteration) {
				break;  /* Reached EOF. */
			}
		}

		return totalFramesRead;
	}


	void _data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		ma_uint16 frameCount_s = frameCount;
		ma_int16* pOutputF16 = (ma_int16*)pOutput;
		ma_uint32 i;

		for (i = 0; i < MIXER_ROWS; i++) {
			if (m_decoder_opts[i].state != MIXER_STOPPED) {
				//ma_uint32 framesRead = read_and_mix_pcm_frames_f16(&m_decoders[i], pOutputF16, frameCount_s);
				ma_uint32 framesRead = _read_and_mix_pcm_frames_f16(m_decoder_refs[i], pOutputF16, frameCount_s, m_decoder_opts[i].volume);
				if (framesRead < frameCount) {
					m_decoder_refs[i] = 0;
					m_decoder_opts[i].state = MIXER_STOPPED;
				}
			}
		}

		(void)pInput;
	}


	void _stop_callback(ma_device* device)
	{
		std::cout << "Audio Device Stopped" << std::endl;
	}


	int next_free_mixer_slot()
	{
		for (int i = 0; i < MIXER_ROWS; i++) {
			if (m_decoder_opts[i].state == MIXER_STOPPED) {
				return i;
			}
		}
		return -1;
	}


	#pragma endregion


	AudioPlayer::AudioPlayer(lua_State* L)
	{
		// create the config used by all sounds.
		m_config = ma_device_config_init(ma_device_type_playback);
		m_config.playback.format = SAMPLE_FORMAT;
		m_config.playback.channels = CHANNEL_COUNT;
		m_config.sampleRate = SAMPLE_RATE;
		m_config.dataCallback = _data_callback;
		m_config.stopCallback = _stop_callback;
		m_alive = false;
		m_playing = false;

		if (ma_device_init(NULL, &m_config, &m_device) != MA_SUCCESS) {
			throw "Failed to open playback device.";
		}

		//m_decoder_opts = (xen_decoder_opts*)malloc(sizeof(*m_decoder_opts) * MIXER_ROWS);

		if (true /* TODO: why?: m_decoder_opts != NULL*/ ) {
			// malloc might return NULL, keep the compiler happy!
			for (int i = 0; i < MIXER_ROWS; i++) {
				m_decoder_refs[i] = 0;
				m_decoder_opts[i].state = MIXER_STOPPED;
				m_decoder_opts[i].volume = 1.0f;
			}
		}

		m_alive = true;
	}


	AudioPlayer::~AudioPlayer()
	{
		if (m_alive) {
			ma_device_stop(&m_device);
			ma_device_uninit(&m_device);
			m_alive = false;
		}
		std::cout << "Deleted instance of AudioPlayer." << std::endl;
	}


	int AudioPlayer::lua_is_playing(lua_State* L)
	{
		int id = lua_tointeger(L, -1);
		xen::Sound* sound = XentuGame::get_instance(L)->assets->get_audio(id);
		bool playing = is_playing(sound);
		lua_pushboolean(L, playing);
		return 1;
	}


	int AudioPlayer::lua_play(lua_State* L)
	{
		int id = lua_tointeger(L, -1);
		xen::Sound* sound = XentuGame::get_instance(L)->assets->get_audio(id);
		play(sound);
		return 0;
	}


	int AudioPlayer::lua_set_volume(lua_State* L) {
		float volume = lua_tonumber(L, -1);
		int id = lua_tointeger(L, -2);
		if (id > 0) {
			xen::Sound* sound = XentuGame::get_instance(L)->assets->get_audio(id);
			auto found = std::find(std::begin(m_decoder_refs), std::end(m_decoder_refs), &sound->decoder);
			if (found != std::end(m_decoder_refs)) {
				int index = found - std::begin(m_decoder_refs);
				m_decoder_opts[index].volume = volume;
			}
		}
		
		return 0;
	}


	int AudioPlayer::lua_stop(lua_State* L)
	{
		int id = lua_tointeger(L, -1);
		xen::Sound* sound = XentuGame::get_instance(L)->assets->get_audio(id);
		stop(sound);
		return 0;
	}


	bool AudioPlayer::is_playing(xen::Sound* sound)
	{
		auto found = std::find(std::begin(m_decoder_refs), std::end(m_decoder_refs), &sound->decoder);
		if (found != std::end(m_decoder_refs)) {
			int index = found - std::begin(m_decoder_refs);
			return m_decoder_opts[index].state == MIXER_PLAYING;
		}
		return false;
	}


	int AudioPlayer::play(Sound* sound)
	{
		// find a free mixer slot.
		int index = next_free_mixer_slot();
		if (index == -1)
			return -1;

		// if the sound is already playing, choose that one instead.
		ma_decoder **found = std::find(std::begin(m_decoder_refs), std::end(m_decoder_refs), &sound->decoder);
		if (found != std::end(m_decoder_refs)) {
			index = std::distance(m_decoder_refs, found);
		}

		// rewind decoder to the start just incase this is not the first time we've used it.
		ma_decoder_seek_to_pcm_frame(&sound->decoder, 0);

		// pUserData is used here to tell the callback which decoder to read from.
		m_decoder_refs[index] = &sound->decoder;
		m_decoder_opts[index].state = MIXER_PLAYING;
		m_device.pUserData = m_decoder_refs[index];

		if (!m_playing) {
			m_playing = true;
			if (ma_device_start(&m_device) != MA_SUCCESS) {
				printf("Failed to start playback device.\n");
				ma_device_uninit(&m_device);
				return -3;
			}
		}

		return 0;
	}


	int AudioPlayer::stop(Sound* sound) {
		auto found = std::find(std::begin(m_decoder_refs), std::end(m_decoder_refs), &sound->decoder);
		if (found != std::end(m_decoder_refs)) {
			int index = found - std::begin(m_decoder_refs);
			//m_decoder_refs[index] = 0;
			m_decoder_opts[index].state = MIXER_STOPPED;
			//ma_decoder_uninit(&sound->decoder);
		}
		return 0;
	}


	const char AudioPlayer::className[] = "AudioPlayer";


	const Luna<AudioPlayer>::PropertyType AudioPlayer::properties[] = {
		{0,0}
	};


	const Luna<AudioPlayer>::FunctionType AudioPlayer::methods[] = {
		method(AudioPlayer, is_playing, lua_is_playing),
		method(AudioPlayer, play, lua_play),
		method(AudioPlayer, set_volume, lua_set_volume),
		method(AudioPlayer, stop, lua_stop),
		{0,0}
	};
}

#endif