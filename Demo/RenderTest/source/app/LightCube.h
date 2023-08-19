#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"

// Draws a cube light
class LightCube : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_shader;
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;

    bool setup() override;

    void render() override;

   public:
    ~LightCube();
};