#ifndef XEN_AUDIO_PLAYER_CPP
#define XEN_AUDIO_PLAYER_CPP

#define MINIAUDIO_IMPLEMENTATION

#include <iostream>
#include <thread>
#include <chrono>

#include "AudioPlayer.hpp"
#include "../XentuGame.hpp"

#define SAMPLE_FORMAT   ma_format_s16
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     44100
#define MIXER_ROWS      8

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name) {#name, &class::name}

namespace xen
{
    ma_decoder *m_decoder_refs[MIXER_ROWS];
    ma_bool32* m_decoders_at_end;
    ma_data_converter_config m_converter_config;
    ma_data_converter m_converter;



    int next_free_mixer_slot()
    {
        for (int i = 0; i < MIXER_ROWS; i++) {
            if (m_decoders_at_end[i] == MA_TRUE) {
                return i;
            }
        }
        return -1;
    }



    void test_callback(ma_device* device)
    {
        std::cout << "Audio Device Stopped" << std::endl;
    }



    AudioPlayer::AudioPlayer(lua_State* L)
    {
        // create the config used by all sounds.
        m_config = ma_device_config_init(ma_device_type_playback);
        m_config.playback.format = SAMPLE_FORMAT;
        m_config.playback.channels = CHANNEL_COUNT;
        m_config.sampleRate = SAMPLE_RATE;
        m_config.dataCallback = AudioPlayer::data_callback;
        m_config.stopCallback = test_callback;
        m_alive = false;
        m_playing = false;

        if (ma_device_init(NULL, &m_config, &m_device) != MA_SUCCESS) {
            throw "Failed to open playback device.";
        }
        
        m_decoders_at_end = (ma_bool32*)malloc(sizeof(*m_decoders_at_end) * MIXER_ROWS);

        if (m_decoders_at_end != NULL) {
            // malloc might return NULL, keep the compiler happy!
            for (int i = 0; i < MIXER_ROWS; i++) {
                m_decoder_refs[i] = 0;
                m_decoders_at_end[i] = MA_TRUE;
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



    int AudioPlayer::play(lua_State* L)
    {
        int id = lua_tointeger(L, -1);
        xen::Sound* sound = XentuGame::get_instance(L)->assets->get_audio(id);
        __play(sound);
        return 1;
    }



    int AudioPlayer::__play(Sound* sound)
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
        m_decoders_at_end[index] = MA_FALSE;
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



    ma_uint16 AudioPlayer::read_and_mix_pcm_frames_f16(ma_decoder* pDecoder, ma_int16* pOutputF16, ma_uint16 frameCount)
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
                pOutputF16[totalFramesRead * CHANNEL_COUNT + iSample] += temp[iSample];
            }

            totalFramesRead += framesReadThisIteration;

            if (framesReadThisIteration < framesToReadThisIteration) {
                break;  /* Reached EOF. */
            }
        }

        return totalFramesRead;
    }



    void AudioPlayer::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        ma_uint16 frameCount_s = frameCount;
        ma_int16* pOutputF16 = (ma_int16*)pOutput;
        ma_uint32 i;

        for (i = 0; i < MIXER_ROWS; i++) {
            if (!m_decoders_at_end[i]) {
                //ma_uint32 framesRead = read_and_mix_pcm_frames_f16(&m_decoders[i], pOutputF16, frameCount_s);
                ma_uint32 framesRead = read_and_mix_pcm_frames_f16(m_decoder_refs[i], pOutputF16, frameCount_s);
                if (framesRead < frameCount) {
                    m_decoder_refs[i] = 0;
                    m_decoders_at_end[i] = MA_TRUE;
                }
            }
        }

        (void)pInput;
    }



    const char AudioPlayer::className[] = "AudioPlayer";



    const Luna<AudioPlayer>::PropertyType AudioPlayer::properties[] = {
        {0,0}
    };



    const Luna<AudioPlayer>::FunctionType AudioPlayer::methods[] = {
        method(AudioPlayer, play),
        {0,0}
    };
}

#endif