#include "kern/graphics/renderer/Sampler.h"

Sampler::Sampler() { glGenSamplers(1, &m_samplerId); }

Sampler::~Sampler() { glDeleteSamplers(1, &m_samplerId); }

void Sampler::bind(GLuint unit) { glBindSampler(unit, m_samplerId); }

void Sampler::setMinFilter(GLint filter)
{
    glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, filter);
}

void Sampler::setMagFilter(GLint filter)
{
    glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, filter);
}