#pragma once

#include <AL/al.h>

#include <cstdlib>

// Represents playable audio data
class Sound
{
   public:
    // pcmSampleData - is either 8 or 16 bit sound data
    // sizeInBytes - total size of the sample data in bytes
    // channels - number of channels, 1 = mono, >=2 = stereo
    // bitsPerSample - size of a single pcm sample in bits, either 8 or 16
    // frequency - the sampling frequency
    Sound(const void* pcmSampleData, std::size_t sizeInBytes, unsigned short channels, unsigned short bitsPerSample,
          unsigned int frequency);
    ~Sound();

    ALuint getBuffer() const;
    ALenum getOALFormat() const;

    float getLength() const;

   private:
    ALuint buffer = 0;
    ALenum format = 0;
};