#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Model.h"
#include "gfx/Shader.h"

// Displays 2 triangles
class ModelLoad : public RenderApplication
{
   private:
    std::vector<Model> models;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> m_mesh;

    bool setup() override;

    void render() override;
};