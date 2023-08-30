#pragma once

#include "gfx/Camera.h"
#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"

class SimpleMoveCamera : public RenderApplication
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
    ~SimpleMoveCamera();
};