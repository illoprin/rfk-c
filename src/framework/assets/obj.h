#pragma once

#include "model.h"

/// @brief parse obj into Model struct
/// allows only obj with: vertices, texcoords, normals
/// allocates vertices and indices arrays (mdl.Vertices, mdl.Indices)
int Mdl_InitFromObj(struct Model* mdl, const char* path);