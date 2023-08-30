#pragma once

#include <glm/glm.hpp>

class Mesh;
class Material;

struct RenderRequest
{
    RenderRequest();
    RenderRequest(Mesh *mesh, Material *material, const glm::mat4 &translation,
                   const glm::mat4 &rotation, const glm::mat4 &scale);

    Mesh *m_mesh;
    Material *m_material;
    glm::mat4 m_translation;
    glm::mat4 m_rotation;
    glm::mat4 m_scale;
};