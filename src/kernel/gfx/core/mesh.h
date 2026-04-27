#pragma once

#include <kernel/gfx/rhi/rhi.h>

struct Mesh {
  struct rhi_VAO VAO;
  struct rhi_Buffer VertexBuf;
  struct rhi_Buffer IndexBuf;
};

struct ModelVertex {
  float Position[3];
  float Texcoords[2];
  float Normal[3];
};

struct Model {
  struct ModelVertex* Vertices;
  uint32_t* Indices;
  size_t NVertices;
  size_t NIndices;
};

void Mesh_SetupBasicQuad(struct Mesh*);
void Mesh_Invalidate(struct Mesh*);