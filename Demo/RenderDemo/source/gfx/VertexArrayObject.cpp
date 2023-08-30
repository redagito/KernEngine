#include "gfx/VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() { glCreateVertexArrays(1, &m_id); }

VertexArrayObject::~VertexArrayObject() { glDeleteVertexArrays(1, &m_id); }

GLuint VertexArrayObject::getId() const { return m_id; }