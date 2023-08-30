#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/Camera.h"

// Draws a cube light
class LightCube : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    GLuint m_vertexArray = 0;
    GLuint m_vertexBuffer = 0;
    Camera m_camera;

    bool setup() override;

    void render() override;

   public:
    ~LightCube();
};