#pragma once

#include <memory>
#include <vector>

#include "kern/graphics/collision/BoundingSphere.h"
#include "kern/graphics/resource/ResourceConfig.h"

#include "kern/graphics/renderer/core/IndexBuffer.h"
#include "kern/graphics/renderer/core/VertexArrayObject.h"
#include "kern/graphics/renderer/core/VertexBuffer.h"

/**
 * \brief Contains mesh data (vertices, faces, normals and uv data).
 *
 * Represents mesh data in the VRAM. It holds a buffer object for vertices,
 * one for texture coordinates and one for nomal data. the data can be used for
 * direct rendering or used in conjunction with an VAO.
 */
class Mesh
{
   public:
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
          const std::vector<float> &normals, const std::vector<float> &uvs, EPrimitiveType type);

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    /**
     * \brief Deletes graphic resources and frees memory.
     */
    ~Mesh();

    /**
     * \brief Initializes mesh with data.
     */
    bool init(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
              const std::vector<float> &normals, const std::vector<float> &uvs,
              EPrimitiveType type);

    /**
     * \brief Returns whether or not an index buffer has been set.
     */
    bool hasIndexBuffer() const;

    /**
     * \brief Read access to vertex buffer
     */
    const std::unique_ptr<VertexBuffer> &getVertexBuffer() const;

    /**
     * \brief Read access to index buffer.
     */
    const std::unique_ptr<IndexBuffer> &getIndexBuffer() const;

    /**
     * \brief Read access to normal buffer
     */
    const std::unique_ptr<VertexBuffer> &getNormalBuffer() const;

    /**
     * \brief Read access to texture uv buffer
     */
    const std::unique_ptr<VertexBuffer> &getUVBuffer() const;

    /**
     * \brief Returns primitive type of the mesh.
     */
    const EPrimitiveType getPrimitiveType() const;

    /**
     * \brief Returns vertex array object.
     */
    const std::unique_ptr<VertexArrayObject> &getVertexArray() const;

    /**
     * \brief Returns bounding sphere for view frustum culling.
     */
    const BoundingSphere &getBoundingSphere() const;

    /**
     * \brief Maps primitive type to GL type.
     * Example: Maps EPrimitiveType::Triangle to GL_TRIANGLES.
     */
    static GLenum toGLPrimitive(EPrimitiveType type);

    /**
     * \brief Returns primitive size for the type.
     * Example: Returns value 3 for EPrimitiveType::Triangle.
     */
    static unsigned int getPrimitiveSize(EPrimitiveType type);

   private:
    std::unique_ptr<VertexBuffer> m_vertices; /**< Mesh vertices. */
    std::unique_ptr<IndexBuffer> m_indices;   /**< Mesh indices. */
    std::unique_ptr<VertexBuffer> m_normals;  /**< Per vertex normals. */
    std::unique_ptr<VertexBuffer> m_uvs;      /**< Texture coordinates. */
    std::unique_ptr<VertexArrayObject> m_vao; /**< Vertex array object. */
    EPrimitiveType m_type;                     /**< Mesh primitive type. */
    // For frustum culling
    // TODO Should be stored separately?
    BoundingSphere m_boundingSphere; /**< Bounding sphere calculated from vertices. */
};
