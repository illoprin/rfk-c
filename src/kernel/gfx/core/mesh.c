#include "mesh.h"

#include <kernel/core/log.h>

void Mesh_SetupBasicQuad(struct Mesh* m) {
  float basicQuadVertices[] = {
    -.5f, -.5f,
    .5f, -.5f,
    .5f, .5f,
    -.5f, .5f,
  };
  uint basicQuadIndices[] = {
    0, 1, 2,
    2, 3, 0,
  };

  // allocate resources
  rhi_Buffer_Create(
    &m->VertexBuf,
    sizeof(basicQuadVertices),
    basicQuadVertices,
    RHI_BUFFER_VERTEX,
    RHI_USAGE_STATIC
  );
  rhi_Buffer_Create(
    &m->IndexBuf,
    sizeof(basicQuadIndices),
    basicQuadIndices,
    RHI_BUFFER_INDEX,
    RHI_USAGE_STATIC
  );

  rhi_VAO_Create(&m->VAO);
  rhi_VAO_BindVertexBuffer(&m->VAO, 0, m->VertexBuf.ID, 0, sizeof(float) * 2);
  rhi_VAO_SetAttribute(&m->VAO,
    (struct rhi_Attribute) {
    .Location = 0,
      .BindingIndex = 0,
      .Components = 2,
      .Type = RHI_ATTR_FLOAT,
      .Normalized = false,
      .Offset = 0,
  });
  rhi_VAO_SetIndexBuffer(&m->VAO, m->IndexBuf.ID);

  RFK_ASSERT(rhi_VAO_IsValid(m->VAO));
}

void Mesh_Invalidate(struct Mesh* m) {
  rhi_VAO_Destroy(&m->VAO);
  rhi_Buffer_Invalidate(&m->VertexBuf);
  rhi_Buffer_Invalidate(&m->IndexBuf);
}
