#pragma once

#include "model.h"

/// @brief parse obj into Model struct
/// allows only obj with: vertices, texcoords, normals
/// allocates vertices and indices arrays (mdl.Vertices, mdl.Indices)
int mdl_init_from_obj(Model* mdl, const char* path);