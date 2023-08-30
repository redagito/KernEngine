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
    glm::vec3 m_lightPos= glm::vec3(1.2f, 1.f, 2.f);

    bool setup() override;

    void render() override;

   public:
    ~BasicLighting();
};