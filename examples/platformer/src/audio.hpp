#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>

#define CHANNELS 2 /* Must be stereo for this example. */
#define SAMPLE_RATE 48000

struct ma_engine;
struct ma_sound;

class Audio
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

#endif