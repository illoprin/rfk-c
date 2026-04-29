#pragma once

#include <kernel/gfx/core/mesh.h>
#include <kernel/core/defs.h>

/// @brief parse obj into Model struct
/// allows only obj with: vertices, texcoords, normals
/// allocates vertices and indices arrays (mdl.Vertices, mdl.Indices)
int Mdl_InitFromObj(struct Model* mdl, const char* path);