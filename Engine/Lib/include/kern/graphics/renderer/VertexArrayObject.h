#pragma once

#include "kern/graphics/renderer/RendererCoreConfig.h"

/**
 * \brief Represents an OpenGl vertex array object.
 * A VAO stores and restores binding state of VBOs.
 */
class VertexArrayObject
{
   public:
    /**
     * \brief Resource allocation.
     */
    VertexArrayObject();

    /**
     * \brief Cleanup of resources.
     */
    ~VertexArrayObject();

    /**
     * \brief Set as active vertex array object.
     */
    void setActive() const;

    /**
     * \brief Disables vertex array object.
     */
    void setInactive() const;

    /**
     * \brief Returns GL object id.
     */
    GLuint getId() const;

   private:
    GLuint m_vaoId; /**< GL VAO id. */
};
