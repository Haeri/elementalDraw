#include "elemd/audio/audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace elemd
{
    Audio::Audio()
    {
        ma_result result;
        _engine = new ma_engine;
        ma_engine_config engineConfig;

        engineConfig = ma_engine_config_init();
        engineConfig.channels = CHANNELS;
        engineConfig.sampleRate = SAMPLE_RATE;

        result = ma_engine_init(&engineConfig, _engine);
        if (result != MA_SUCCESS)
        {
            printf("Failed to initialize audio engine.");
            return;
        }
    }

    Audio::~Audio()
    {
        for (auto sound : _sounds)
        {
            ma_sound_uninit(sound);
            delete sound;
        }

        ma_engine_uninit(_engine);
        delete _engine;
    }

    void Audio::registerSound(std::string filename, std::string name)
    {
        ma_sound* s = new ma_sound;
        _sounds.push_back(s);
        ma_result result = ma_sound_init_from_file(_engine, filename.c_str(), 0, NULL, NULL, s);
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
} // namespace elemd