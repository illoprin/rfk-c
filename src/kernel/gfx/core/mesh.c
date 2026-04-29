#include "mesh.h"
#include <kernel/core/log.h>
#include <stddef.h>

void Mesh_SetupFromModel(struct Mesh* mesh, struct Model* model) {
  if (mesh->VAO.ID > 0) {
    return;
  }

  size_t sizeOfVertices = sizeof(struct ModelVertex) * model->NumVertices;
  size_t sizeOfIndices = sizeof(int) * model->NumIndices;

  // allocate vertices
  rhi_Buf_Init(&mesh->VertexBuf, RHI_BUF_VERTEX, RHI_USAGE_STATIC);
  rhi_Buf_Allocate(&mesh->VertexBuf, model->Vertices, sizeOfVertices);

  // allocate indices
  rhi_Buf_Init(&mesh->IndexBuf, RHI_BUF_INDEX, RHI_USAGE_STATIC);
  rhi_Buf_Allocate(&mesh->IndexBuf, model->Indices, sizeOfIndices);

  // init attributes
  struct rhi_Attribute pos = { 0 };
  struct rhi_Attribute tex = { 0 };
  struct rhi_Attribute nrm = { 0 };

  // position
  pos.Location = 0;
  pos.Type = RHI_FLOAT;
  pos.Components = 3;
  pos.Offset = 0;
  pos.Stride = sizeof(struct ModelVertex);

  // texcoord
  tex.Location = 1;
  tex.Components = 2;
  tex.Type = RHI_FLOAT;
  tex.Offset = offsetof(struct ModelVertex, Texcoords[0]);
  tex.Stride = sizeof(struct ModelVertex);

  // normal
  nrm.Location = 2;
  nrm.Components = 3;
  nrm.Type = RHI_FLOAT;
  nrm.Offset = offsetof(struct ModelVertex, Normal[0]);
  nrm.Stride = sizeof(struct ModelVertex);

  struct rhi_Attribute attrs[] = { pos, tex, nrm };

  // create vao
  rhi_VAO_Init(&mesh->VAO);
  rhi_VAO_AddIndexBuffer(&mesh->VAO, mesh->IndexBuf);
  rhi_VAO_AddAttributes(&mesh->VAO, mesh->VertexBuf, attrs, 3);
  RFK_ASSERT(rhi_VAO_IsValid(mesh->VAO));

  mesh->indexNum = model->NumIndices;
}
void Mesh_SetupBasicQuad(struct Mesh* m) {
  if (m->VAO.ID > 0) {
    return;
  }
  float verts[] = {
    -.5f, -.5f,
    .5f, -.5f,
    .5f, .5f,
    -.5f, .5f,
  };
  int indices[] = {
    0, 1, 2,
    2, 3, 0,
  };

  // allocate vertices
  rhi_Buf_Init(&m->VertexBuf, RHI_BUF_VERTEX, RHI_USAGE_STATIC);
  rhi_Buf_Allocate(&m->VertexBuf, verts, sizeof(verts));

  // allocate indices
  rhi_Buf_Init(&m->IndexBuf, RHI_BUF_INDEX, RHI_USAGE_STATIC);
  rhi_Buf_Allocate(&m->IndexBuf, indices, sizeof(indices));

  struct rhi_Attribute position = { 0 };
  position.Location = 0;
  position.Components = 2;
  position.Offset = 0;
  position.Stride = 2 * sizeof(float);
  position.Type = RHI_FLOAT;

  // create vao
  rhi_VAO_Init(&m->VAO);
  rhi_VAO_AddIndexBuffer(&m->VAO, m->IndexBuf);
  rhi_VAO_AddAttributes(&m->VAO, m->VertexBuf, &position, 1);
  RFK_ASSERT(rhi_VAO_IsValid(m->VAO));

  m->indexNum = 6;
}

void Mesh_Invalidate(struct Mesh* m) {
  rhi_VAO_Invalidate(&m->VAO);
  rhi_Buf_Invalidate(&m->VertexBuf);
  rhi_Buf_Invalidate(&m->IndexBuf);
}
