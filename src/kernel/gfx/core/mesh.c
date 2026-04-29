#include "mesh.h"
#include <kernel/core/log.h>
#include <stddef.h>

void Mesh_SetupFromModel(struct Mesh* mesh, struct Model* model) {
  if (mesh->VAO.ID > 0) {
    return;
  }

  size_t sizeOfVertices = sizeof(struct ModelVertex) * model->NumVertices;
  size_t sizeOfIndices = sizeof(uint) * model->NumIndices;

  // create buffer
  rhi_Buffer_Create(
    &mesh->VertexBuf,
    sizeOfVertices,
    model->Vertices,
    RHI_BUFFER_VERTEX,
    RHI_USAGE_STATIC
  );
  rhi_Buffer_Create(
    &mesh->IndexBuf,
    sizeOfIndices,
    model->Indices,
    RHI_BUFFER_INDEX,
    RHI_USAGE_STATIC
  );

  // setup attributes

  // position
  struct rhi_Attribute positionAttr = { 0 };
  positionAttr.Location = 0;
  positionAttr.BindingIndex = 0;
  positionAttr.Components = 3;
  positionAttr.Type = RHI_FLOAT;
  positionAttr.Offset = 0;

  // texcoord
  struct rhi_Attribute texcoordAttr = { 0 };
  texcoordAttr.Location = 1;
  texcoordAttr.BindingIndex = 0;
  texcoordAttr.Components = 2;
  texcoordAttr.Type = RHI_FLOAT;
  texcoordAttr.Offset = sizeof(float) * 3;

  // normal
  struct rhi_Attribute normalAttr = { 0 };
  normalAttr.Location = 2;
  normalAttr.BindingIndex = 0;
  normalAttr.Components = 3;
  normalAttr.Type = RHI_FLOAT;
  normalAttr.Offset = sizeof(float) * 5;

  // create vao
  rhi_VAO_Create(&mesh->VAO);
  rhi_VAO_BindVertexBuffer(&mesh->VAO, 0, mesh->VertexBuf.ID, 0, 8 * sizeof(float));
  rhi_VAO_SetAttribute(&mesh->VAO, positionAttr);
  rhi_VAO_SetAttribute(&mesh->VAO, texcoordAttr);
  rhi_VAO_SetAttribute(&mesh->VAO, normalAttr);
  rhi_VAO_SetIndexBuffer(&mesh->VAO, mesh->IndexBuf.ID);

  mesh->indexNum = model->NumIndices;

  RFK_ASSERT(rhi_VAO_IsValid(mesh->VAO));
}
void Mesh_SetupBasicQuad(struct Mesh* m) {
  if (m->VAO.ID > 0) {
    return;
  }
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
      .Type = RHI_FLOAT,
      .Offset = 0,
  });
  rhi_VAO_SetIndexBuffer(&m->VAO, m->IndexBuf.ID);

  m->indexNum = 6;

  RFK_ASSERT(rhi_VAO_IsValid(m->VAO));
}

void Mesh_Invalidate(struct Mesh* m) {
  rhi_VAO_Destroy(&m->VAO);
  rhi_Buffer_Invalidate(&m->VertexBuf);
  rhi_Buffer_Invalidate(&m->IndexBuf);
}
