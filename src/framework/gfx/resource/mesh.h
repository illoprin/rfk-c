#pragma once

#include <framework/assets/model.h>
#include <kernel/gfx/rhi/rhi.h>

typedef struct {
  rhi_VAO    VAO;
  rhi_Buffer VertexBuf;
  rhi_Buffer IndexBuf;
  int        indexNum;
} Mesh;

void mesh_init_basic_quad(Mesh*);
void mesh_init_from_model(Mesh* mesh, Model* model);
void mesh_invalidate(Mesh*);