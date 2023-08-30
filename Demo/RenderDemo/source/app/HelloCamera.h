#pragma once

#include "RenderApplication.h"
#include "gfx/Camera.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"

class HelloCamera : public RenderApplication
{
   private:
    std::unique_ptr<Shader> shader;
    Texture texContainer;
    Camera camera;
    GLuint vao = 0;
    GLuint vbo = 0;

    bool setup() override;

    void render() override;

   public:
    ~HelloCamera();
};