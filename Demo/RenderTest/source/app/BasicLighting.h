#pragma once

#include "RenderApplication.h"
#include "gfx/Camera.h"
#include "gfx/Shader.h"

class BasicLighting : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    GLuint m_vertexArray = 0;
    GLuint m_vertexBuffer = 0;

    bool setup() override;

    void render() override;

   public:
    ~BasicLighting();
};