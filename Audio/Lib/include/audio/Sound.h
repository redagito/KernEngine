#pragma once

#include <AL/al.h>

#include <cstdint>

// Represents playable audio data
class Sound
{
   public:
    Sound(const char* data, unsigned int size, unsigned short channels, unsigned short samples,
          unsigned int frequency);
    ~Sound();

    ALuint getBuffer() const;
    ALenum getOALFormat() const;

    float getLength() const;

   private:
    ALuint buffer = 0;
    ALenum format = 0;
};