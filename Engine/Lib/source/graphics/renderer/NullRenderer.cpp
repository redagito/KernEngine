#include "kern/graphics/renderer/NullRenderer.h"

#include <cassert>

#include <fmtlog/fmtlog.h>

#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/IWindow.h"
#include "kern/graphics/renderer/core/RendererCoreConfig.h"
#include "kern/graphics/renderer/debug/RendererDebug.h"

void NullRenderer::draw(const IScene &scene, const ICamera &camera, const IWindow &window,
                         const IGraphicsResourceManager &manager)
{
    window.setActive();
    // Set clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return;
}

NullRenderer *NullRenderer::create() { return new NullRenderer; }