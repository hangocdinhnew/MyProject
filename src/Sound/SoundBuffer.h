#pragma once
#include "../Config/MPConfig.h"
#include <AL/al.h>

class SoundBuffer
{
    public:
        static SoundBuffer* get();

        ALuint addSoundEffect(const char* filename);
        bool removeSoundEffect(const ALuint& buffer);

    private:
        SoundBuffer();
        ~SoundBuffer();
    
    std::vector<ALuint> p_SoundEffectBuffers;
};