#pragma once

#include "RenderApplication.h"

// Displays 2 triangles
class ImguiDemo : public RenderApplication
{
   private:
    bool setup() override;

    void render() override;

   public:
    ~ImguiDemo();
};