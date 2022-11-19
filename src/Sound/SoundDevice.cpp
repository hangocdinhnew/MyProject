#include "SoundDevice.h"

SoundDevice* SoundDevice::get()
{
    static SoundDevice* snd_device = new SoundDevice();
    return snd_device;
}

SoundDevice::SoundDevice()
{
    p_ALCDevice = alcOpenDevice(nullptr);
    if (!p_ALCDevice)
        throw("ERROR: Unable to get sound device CHECK YOUR SOUND DEVICE OR ADD A SOUND CARD OR A SPEAKER TO EXPRIENCE THE AUDIO");

    p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
    if (!p_ALCContext)
        throw("FAILED TO MAKE SOUND CONTEXT");

    if (!alcMakeContextCurrent(p_ALCContext))
        throw("FAILED TO MAKE CONTEXT CURRENT");

    const ALCchar* name = nullptr;
    if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXIT"))
        name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
        name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
    printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
    if (!alcMakeContextCurrent(nullptr))
        throw("failed to set context to nullptr");
    
    alcDestroyContext(p_ALCContext);
    if (p_ALCContext)
        throw("failed to unset during close");
    
    if (!alcCloseDevice(p_ALCDevice))
        throw("failed to close sound device");
}