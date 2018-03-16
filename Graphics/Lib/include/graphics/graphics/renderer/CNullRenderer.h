#pragma once

#include "graphics/graphics/IRenderer.h"

class CNullRenderer : public IRenderer
{
   public:
    /**
     * \brief Performs no actual drawing operations.
     *
     * Clears screen to black.
     */
    void draw(const IScene &scene, const ICamera &camera, const IWindow &window,
              const IGraphicsResourceManager &manager);

    /**
     * \brief Creates and initializes null renderer.
     */
    static CNullRenderer *create();
};