#include "graphics/graphics/renderer/Draw.h"

#include <cassert>

#include <foundation/debug/Log.h>

#include "graphics/graphics/resource/CMesh.h"

void draw(CMesh &mesh)
{
    if (mesh.getPrimitiveType() == EPrimitiveType::Invalid)
    {
        return;
    }
    mesh.getVertexArray()->setActive();

    // Set primitive draw mode
    GLenum mode = CMesh::toGLPrimitive(mesh.getPrimitiveType());
    unsigned int primitiveSize = CMesh::getPrimitiveSize(mesh.getPrimitiveType());

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