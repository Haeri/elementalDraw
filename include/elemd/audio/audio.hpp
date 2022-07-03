#ifndef ELEMD_AUDIO_H
#define ELEMD_AUDIO_H

#include <vector>
#include <string>

#include <elemd/elemental_draw.hpp>

#define CHANNELS 2 /* Must be stereo for this example. */
#define SAMPLE_RATE 48000

struct ma_engine;
struct ma_sound;

namespace elemd
{
    class ELEMD_API Audio
    {
    public:
        Audio();
        ~Audio();

        void registerSound(std::string filename, std::string name);
        void playSound(std::string name);

    private:
        ma_engine* _engine;
        std::vector<ma_sound*> _sounds;
    };
} // namespace elemd

#endif // ELEMD_AUDIO_HPP