#include "kern/graphics/renderer/VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &m_vaoId); }

VertexArrayObject::~VertexArrayObject() { glDeleteVertexArrays(1, &m_vaoId); }

void VertexArrayObject::setActive() const
{
    glBindVertexArray(m_vaoId);
    return;
}

void VertexArrayObject::setInactive() const
{
    glBindVertexArray(0);
    return;
}

GLuint VertexArrayObject::getId() const { return m_vaoId; }
