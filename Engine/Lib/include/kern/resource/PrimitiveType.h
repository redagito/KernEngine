#pragma once

/**
 * \brief Primitive type for vertex data.
 * NOTE GL_QUAD is deprecated.
 */
enum class PrimitiveType
{
    Point = 1,
    Line = 2,
    Triangle = 3,
    Invalid
};