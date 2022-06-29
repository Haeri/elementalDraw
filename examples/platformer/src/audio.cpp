#include "audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


/*
void data_callback(void* pUserData, ma_uint8* pBuffer, int bufferSizeInBytes)
{
    /* Reading is just a matter of reading straight from the engine. * /
    ma_uint32 bufferSizeInFrames =
        (ma_uint32)bufferSizeInBytes /
        ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&g_engine));
    ma_engine_read_pcm_frames(&g_engine, pBuffer, bufferSizeInFrames, NULL);
}
*/

Audio::Audio()
{
    ma_result result;
    //ma_engine_config engineConfig;

    //engineConfig = ma_engine_config_init();
    //engineConfig.channels = CHANNELS;
    //engineConfig.sampleRate = SAMPLE_RATE;

    result = ma_engine_init(NULL, _engine);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize audio engine.");
        return;
    }
}

void Audio::registerSound(std::string filename, std::string name)
{
    ma_result result = ma_sound_init_from_file(_engine, filename.c_str(), 0, NULL, NULL,
                                               *(_sounds.data()));
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize sound.");
        return;
    }
}

void Audio::playSound(std::string name)
{
    ma_engine_play_sound(_engine, name.c_str(), NULL);
}
