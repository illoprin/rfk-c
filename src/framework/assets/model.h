#pragma once

#include <rfklib/log.h>
#include <stdint.h>

typedef struct {
  float Position[3];
  float Texcoords[2];
  float Normal[3];
} ModelVertex;

typedef struct {
  // vertices
  struct ModelVertex* Vertices;
  size_t              CountVertices;

  // indices
  uint32_t* Indices;
  size_t    CountIndices;
} Model;

void mdl_ptr_free(Model*);
void mdl_free(Model);