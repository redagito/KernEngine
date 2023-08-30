#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Shader.h"

/**
 * Display 2 colored triangles
 */
class ColorTriangle : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_shader;
    GLuint m_vertexArray = 0;
    GLuint m_vertexBuffer = 0;
    GLuint m_indexBuffer = 0;

    bool setup() override;

    void render() override;

   public:
    ~ColorTriangle();
};