#include "audio/Sound.h"

#include <cstdint>
#include <stdexcept>

ALenum toALFormat(unsigned short channels, unsigned short samples) { 
    bool stereo = channels > 1;
    if (samples == 16 && stereo)
        return AL_FORMAT_STEREO16;
    if (samples == 16)
        return AL_FORMAT_MONO16;

    if (samples == 8 && stereo)
        return AL_FORMAT_STEREO8;
    if (samples == 8)
        return AL_FORMAT_MONO8;

    throw std::runtime_error("Invalid format");
}

Sound::Sound(const char* data, unsigned int size, unsigned short channels, unsigned short samples, unsigned int frequency) 
{ 
    alGenBuffers(1, &buffer);
    format = toALFormat(channels, samples);
    alBufferData(buffer, format, data, size, frequency);
}

Sound::~Sound() { alDeleteBuffers(1, &buffer); }

ALuint Sound::getBuffer() const { return buffer; }

ALenum Sound::getOALFormat() const { return format; }

float Sound::getLength() const { return 0.f; }