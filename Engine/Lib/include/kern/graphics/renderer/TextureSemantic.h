#pragma once

enum class TextureSemantic
{
    DiffuseRGB,  /**< Stores diffuse rgb data. */
    SceneRGBLDR, /**< Stores currently lit scene RGB data in LDR format. */
    SceneRGBHDR, /**< Stores currently lit scene RGB data in HDR format. */
    Depth,       /**< Stores depth texture. */

    Unknown /**< Unknown texture semantic. */
};