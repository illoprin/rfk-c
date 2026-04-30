#pragma once

#include <kernel/core/types.h>
#include <stdint.h>

struct ModelVertex {
  float Position[3];
  float Texcoords[2];
  float Normal[3];
};

struct Model {
  // vertices
  struct ModelVertex* Vertices;
  size_t NumVertices;

  // indices
  uint32_t* Indices;
  size_t NumIndices;
};

void MdlPtr_Free(struct Model*);
void Mdl_Free(struct Model);