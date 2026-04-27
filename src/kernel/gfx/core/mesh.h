#pragma once

#include <kernel/gfx/rhi/rhi.h>

struct Mesh {
  struct rhi_VAO VAO;
  struct rhi_Buffer VertexBuf;
  struct rhi_Buffer IndexBuf;
};

void Mesh_SetupBasicQuad(struct Mesh*);
void Mesh_Invalidate(struct Mesh*);