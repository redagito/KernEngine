#pragma once

class CMesh;

/**
 * \brief Performs GL draw call based on mesh data.
 *
 * Sets vertex array object active and performs either indexed or non-indexed
 *draw call.
 * Shader must be set by caller.
 */
void draw(CMesh &mesh);