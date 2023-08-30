#pragma once

#include <glad/glad.h>

class VertexArrayObject
{
   public:
    VertexArrayObject();
    ~VertexArrayObject();

    GLuint getId() const;

   private:
    GLuint m_id = 0;
};