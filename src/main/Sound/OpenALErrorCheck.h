#pragma once
#include <AL/al.h>
#include <AL/alc.h>

static void ALC_CheckAndThrow(ALCdevice *device) {
  if (alcGetError(device) != ALC_NO_ERROR) {
    throw("error with alcDevice");
  }
}

static void AL_CheckAndThrow() {
  if (alGetError() != AL_NO_ERROR) {
    throw("error with al");
  }
}