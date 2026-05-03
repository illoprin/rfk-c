#include "mesh.h"
#include <rfklib/log.h>
#include <stddef.h>

void mesh_init_from_model(Mesh* mesh, Model* model) {
  if (mesh->VAO.ID > 0) { return; }

  size_t sizeOfVertices = sizeof(ModelVertex) * model->CountVertices;
  size_t sizeOfIndices  = sizeof(int) * model->CountIndices;

  // allocate vertices
  rhi_buf_init(&mesh->VertexBuf, RHI_BUF_VERTEX, RHI_USAGE_STATIC);
  rhi_buf_allocate(&mesh->VertexBuf, model->Vertices, sizeOfVertices);

  // allocate indices
  rhi_buf_init(&mesh->IndexBuf, RHI_BUF_INDEX, RHI_USAGE_STATIC);
  rhi_buf_allocate(&mesh->IndexBuf, model->Indices, sizeOfIndices);

  // init attributes
  rhi_Attribute pos = {0};
  rhi_Attribute tex = {0};
  rhi_Attribute nrm = {0};

  // position
  pos.Location   = 0;
  pos.Type       = RHI_FLOAT;
  pos.Components = 3;
  pos.Offset     = 0;
  pos.Stride     = sizeof(ModelVertex);

  // texcoord
  tex.Location   = 1;
  tex.Components = 2;
  tex.Type       = RHI_FLOAT;
  tex.Offset     = offsetof(ModelVertex, Texcoords[0]);
  tex.Stride     = sizeof(ModelVertex);

  // normal
  nrm.Location   = 2;
  nrm.Components = 3;
  nrm.Type       = RHI_FLOAT;
  nrm.Offset     = offsetof(ModelVertex, Normal[0]);
  nrm.Stride     = sizeof(ModelVertex);

  rhi_Attribute attrs[] = {pos, tex, nrm};

  // create vao
  rhi_vao_init(&mesh->VAO);
  rhi_vao_add_index_buf(&mesh->VAO, mesh->IndexBuf);
  rhi_vao_add_attrs(&mesh->VAO, mesh->VertexBuf, attrs, 3);
  RFK_ASSERT(rhi_vao_is_valid(mesh->VAO), "invalid mesh");

  mesh->indexNum = model->CountIndices;
}

void mesh_init_basic_quad(Mesh* m) {
  if (m->VAO.ID > 0) { return; }
  float verts[] = {
    -1.f,
    -1.f,
    1.f,
    -1.f,
    1.f,
    1.f,
    -1.f,
    1.f,
  };
  int indices[] = {
    0,
    1,
    2,
    2,
    3,
    0,
  };

  // allocate vertices
  rhi_buf_init(&m->VertexBuf, RHI_BUF_VERTEX, RHI_USAGE_STATIC);
  rhi_buf_allocate(&m->VertexBuf, verts, sizeof(verts));

  // allocate indices
  rhi_buf_init(&m->IndexBuf, RHI_BUF_INDEX, RHI_USAGE_STATIC);
  rhi_buf_allocate(&m->IndexBuf, indices, sizeof(indices));

  rhi_Attribute position = {0};
  position.Location      = 0;
  position.Components    = 2;
  position.Offset        = 0;
  position.Stride        = 2 * sizeof(float);
  position.Type          = RHI_FLOAT;

  // create vao
  rhi_vao_init(&m->VAO);
  rhi_vao_add_index_buf(&m->VAO, m->IndexBuf);
  rhi_vao_add_attrs(&m->VAO, m->VertexBuf, &position, 1);
  RFK_ASSERT(rhi_vao_is_valid(m->VAO), "invalid mesh");

  m->indexNum = 6;
}

void mesh_invalidate(Mesh* m) {
  rhi_vao_invalidate(&m->VAO);
  rhi_buf_invalidate(&m->VertexBuf);
  rhi_buf_invalidate(&m->IndexBuf);
}
