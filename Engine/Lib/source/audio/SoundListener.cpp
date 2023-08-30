#include "kern/audio/SoundListener.h"

#include "AL/al.h"

void SoundListener::setPosition(const glm::vec3& pos)
{
    position = pos;
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void SoundListener::setVelocity(const glm::vec3& velo)
{
    velocity = velo;
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

const glm::vec3& SoundListener::getPosition() const { return position; }
const glm::vec3& SoundListener::getVelocity() const { return velocity; }