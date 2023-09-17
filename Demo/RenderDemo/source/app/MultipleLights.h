#pragma once

#include "RenderApplication.h"
#include "gfx/DirectionalLight.h"
#include "gfx/PointLight.h"
#include "gfx/SpotLight.h"
#include "gfx/Shader.h"
#include "gfx/VertexArrayObject.h"
#include "gfx/VertexBuffer.h"

class MultipleLights : public RenderApplication
{
   public:
    ~MultipleLights() = default;

   private:
    bool setup() override;
    void render() override;

    // Scene data
    // Lights
    DirectionalLight m_dirLight;
    SpotLight m_spotLight;
    PointLight m_pointLights[4];
    // Model positions
    std::vector<glm::vec3> m_cubePositions;

    // Graphics resources
    // Shader
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    // Mesh data
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    VertexArrayObject m_vao;
    // textures
    std::shared_ptr<Texture> m_diffuse;
    std::shared_ptr<Texture> m_specular;
};