#pragma once

#include <kernel/gfx/rhi/rhi.h>
#include <kernel/gfx/core/model.h>

struct Mesh {
  struct rhi_VAO VAO;
  struct rhi_Buffer VertexBuf;
  struct rhi_Buffer IndexBuf;
  int indexNum;
};

void Mesh_SetupBasicQuad(struct Mesh*);
void Mesh_SetupFromModel(struct Mesh* mesh, struct Model* model);
void Mesh_Invalidate(struct Mesh*);