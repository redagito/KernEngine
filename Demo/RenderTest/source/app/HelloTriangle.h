#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Shader.h"

// Displays 2 triangles
class HelloTriangle : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_shader;
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;

    bool setup() override;

    void render() override;

   public:
    ~HelloTriangle();
};