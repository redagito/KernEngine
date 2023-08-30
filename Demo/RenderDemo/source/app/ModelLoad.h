#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Model.h"
#include "gfx/Shader.h"
#include "gfx/Camera.h"

// Displays 2 triangles
class ModelLoad : public RenderApplication
{
   private:
    std::vector<Model> models;
    std::unique_ptr<Shader> shader;

    bool setup() override;

    void render() override;
};