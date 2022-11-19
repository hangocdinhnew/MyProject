#pragma once
#include "../Config/MPConfig.h"
#include <AL/alc.h>
#include <AL/al.h>

class SoundDevice
{
    public:
        static SoundDevice* get();
    private:
        SoundDevice();
        ~SoundDevice();
    
    ALCdevice* p_ALCDevice;
    ALCcontext* p_ALCContext;
};