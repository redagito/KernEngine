#pragma once

#include "kern/graphics/renderer/core/RendererCoreConfig.h"

/**
 * \brief Opengl sampler wrapper.
 */
class Sampler
{
   public:
    Sampler();
    ~Sampler();

    /**
     * \brief Bind to texture unit.
     */
    void bind(GLuint unit);

    void setMinFilter(GLint filter);
    void setMagFilter(GLint filter);

   private:
    GLuint m_samplerId = 0;
};