#include "mesh.h"
#include <kernel/core/log.h>
#include <stddef.h>

static void printAttr(struct rhi_Attribute attr) {
  printf("{\n"
    "\tLocation: %d\n"
    "\tBinding Index: %d\n"
    "\tComponents: %d\n"
    "\tOffset: %d\n"
    "\tDivisor:%d\n"
    "}\n",
    attr.Location,
    attr.BindingIndex,
    attr.Components,
    attr.Offset,
    attr.Divisor
  );
}

void Mesh_FromModel(struct Mesh* mesh, struct Model* model) {

  size_t sizeOfVertices = sizeof(struct ModelVertex) * model->NVertices;
  size_t sizeOfIndices = sizeof(uint32_t) * model->NIndices;

  // create buffers
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
  positionAttr.Type = RHI_ATTR_FLOAT;
  positionAttr.Offset = 0;

  printAttr(positionAttr);

  // texcoord
  struct rhi_Attribute texcoordAttr = { 0 };
  texcoordAttr.Location = 1;
  texcoordAttr.BindingIndex = 0;
  texcoordAttr.Components = 2;
  texcoordAttr.Type = RHI_ATTR_FLOAT;
  texcoordAttr.Offset = offsetof(struct ModelVertex, Texcoords);

  // normal
  struct rhi_Attribute normalAttr = { 0 };
  normalAttr.Location = 2;
  normalAttr.BindingIndex = 0;
  normalAttr.Components = 3;
  normalAttr.Type = RHI_ATTR_FLOAT;
  normalAttr.Offset = offsetof(struct ModelVertex, Normal);

  // create vao
  rhi_VAO_Create(&mesh->VAO);
  rhi_VAO_BindVertexBuffer(&mesh->VAO, 0, mesh->VertexBuf.ID, 0, sizeof(struct ModelVertex));
  rhi_VAO_SetAttribute(&mesh->VAO, positionAttr);
  rhi_VAO_SetAttribute(&mesh->VAO, texcoordAttr);
  rhi_VAO_SetAttribute(&mesh->VAO, normalAttr);
  rhi_VAO_SetIndexBuffer(&mesh->VAO, mesh->IndexBuf.ID);

  RFK_ASSERT(rhi_VAO_IsValid(mesh->VAO));
}

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
