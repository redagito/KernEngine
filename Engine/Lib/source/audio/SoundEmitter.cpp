#include "kern/audio/SoundEmitter.h"

SoundEmitter::SoundEmitter() { alGenSources(1, &source); }
SoundEmitter::~SoundEmitter() { alDeleteSources(1, &source); }

void SoundEmitter::play(const std::shared_ptr<Sound>& sound) 
{ 
    alSourcei(source, AL_BUFFER, sound->getBuffer());
    alSourcePlay(source);
}

void SoundEmitter::stop() { alSourceStop(source); }

void SoundEmitter::reset()
{

}

void SoundEmitter::setPosition(const glm::vec3& pos)
{
    position = pos;
    alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}

const glm::vec3& SoundEmitter::getPosition() const { return position; }

void SoundEmitter::setVelocity(const glm::vec3& vel)
{
    velocity = vel;
    alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

const glm::vec3& SoundEmitter::getVelocity() const { return velocity; }

SoundPriority SoundEmitter::getPriority() const { return priority; }

void SoundEmitter::setPriority(SoundPriority prio) { priority = prio; }

float SoundEmitter::getVolume() const { return volume; }

void SoundEmitter::setVolume(float vol) { volume = vol; }

bool SoundEmitter::getLooping() const { return looping; }

void SoundEmitter::setLooping(bool loop)
{
    looping = loop;
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

float SoundEmitter::getRadius() const { return radius; }

void SoundEmitter::setRadius(float radi) { radius = radi; }