#include "kern/audio/Sound.h"

#include <cstdint>
#include <stdexcept>
#include <fmtlog/fmtlog.h>

ALenum toALFormat(unsigned short channels, unsigned short bitsPerSample)
{ 
    bool stereo = channels > 1;

    if (bitsPerSample == 8 && stereo)
        return AL_FORMAT_STEREO8;
    if (bitsPerSample == 8)
        return AL_FORMAT_MONO8;

    if (bitsPerSample == 16 && stereo)
        return AL_FORMAT_STEREO16;
    if (bitsPerSample == 16)
        return AL_FORMAT_MONO16;

    throw std::runtime_error("Invalid format");
}

Sound::Sound(const void* pcmSampleData, std::size_t sizeInBytes, unsigned short channels, unsigned short bitsPerSample,
             unsigned int frequency)
{ 
    logd("Creating sound with {} bytes, {} channels, {} bits per sample and frequency of {} hz", sizeInBytes, channels, bitsPerSample, frequency);
    alGenBuffers(1, &buffer);
    format = toALFormat(channels, bitsPerSample);
    alBufferData(buffer, format, pcmSampleData, sizeInBytes, frequency);
}

Sound::~Sound() { alDeleteBuffers(1, &buffer); }

ALuint Sound::getBuffer() const { return buffer; }

ALenum Sound::getOALFormat() const { return format; }

float Sound::getLength() const { return 0.f; }