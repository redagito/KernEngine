#include "kern/graphics/renderer/Draw.h"

#include <cassert>

#include <fmtlog/fmtlog.h>

#include "kern/graphics/resource/Mesh.h"

void draw(Mesh &mesh)
{
    if (mesh.getPrimitiveType() == PrimitiveType::Invalid)
    {
        return;
    }
    mesh.getVertexArray()->setActive();

    // Set primitive draw mode
    GLenum mode = Mesh::toGLPrimitive(mesh.getPrimitiveType());
    unsigned int primitiveSize = Mesh::getPrimitiveSize(mesh.getPrimitiveType());

    // Decide on draw method based on the stored data
    if (mesh.hasIndexBuffer())
    {
        // Indexed draw, faster
        mesh.getIndexBuffer()->setActive();
        glDrawElements(mode, mesh.getIndexBuffer()->getSize(), GL_UNSIGNED_INT, nullptr);
        mesh.getIndexBuffer()->setInactive();
    }
    else
    {
        // Slowest draw method
        glDrawArrays(mode, 0, mesh.getVertexBuffer()->getSize() / primitiveSize);
    }
    mesh.getVertexArray()->setInactive();
}