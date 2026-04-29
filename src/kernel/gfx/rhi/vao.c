#include "vao.h"
#include <kernel/core/log.h>

// -----------------------------
// Create / Destroy
// -----------------------------

void rhi_VAO_Create(struct rhi_VAO* vao) {
  RFK_ASSERT(vao != NULL);

  glCreateVertexArrays(1, &vao->ID);

  vao->ebo_id = 0;

  LogInfo("VAO [ID: %u] created", vao->ID);
}

void rhi_VAO_Destroy(struct rhi_VAO* vao) {
  if (!vao || vao->ID == 0)
    return;

  glDeleteVertexArrays(1, &vao->ID);
  LogInfo("VAO [ID: %u] deleted", vao->ID);

  vao->ID = 0;
  vao->ebo_id = 0;
}

// -----------------------------
// Bind
// -----------------------------

void rhi_VAO_Bind(struct rhi_VAO* vao) {
  RFK_ASSERT(vao && vao->ID);
  glBindVertexArray(vao->ID);
}

// -----------------------------
// REQUIRED EBO
// -----------------------------

void rhi_VAO_SetIndexBuffer(struct rhi_VAO* vao, uint32_t ebo_id) {
  RFK_ASSERT(vao && vao->ID);
  RFK_ASSERT(ebo_id != 0);

  glVertexArrayElementBuffer(vao->ID, ebo_id);

  vao->ebo_id = ebo_id;
}

// -----------------------------
// Vertex buffer binding
// -----------------------------

void rhi_VAO_BindVertexBuffer(
  struct rhi_VAO* vao,
  uint32_t bindingIdx,
  uint32_t vbo_id,
  size_t offset,
  size_t stride
) {
  RFK_ASSERT(vao && vao->ID);
  RFK_ASSERT(vbo_id != 0);

  glVertexArrayVertexBuffer(
    vao->ID,
    bindingIdx,
    vbo_id,
    offset,
    stride
  );
}

// -----------------------------
// Attribute setup
// -----------------------------

void rhi_VAO_SetAttribute(
  struct rhi_VAO* vao,
  struct rhi_Attribute attr
) {
  RFK_ASSERT(vao && vao->ID);

  glEnableVertexArrayAttrib(vao->ID, attr.Location);

  if (attr.Type != RHI_FLOAT) {
    // for integer (has no normalization)
    glVertexArrayAttribIFormat(
      vao->ID,
      attr.Location,
      attr.Components,
      (GLenum)attr.Type,
      (GLuint)attr.Offset
    );
  }
  else {
    // for floats (with normalization)
    glVertexArrayAttribFormat(
      vao->ID,
      attr.Location,
      attr.Components,
      (GLenum)attr.Type,
      GL_FALSE,
      (GLuint)attr.Offset
    );
  }

  glVertexArrayAttribBinding(
    vao->ID,
    attr.Location,
    attr.BindingIndex
  );

  glVertexArrayBindingDivisor(vao->ID, attr.BindingIndex, attr.Divisor);
}

// -----------------------------
// Validation
// -----------------------------

bool rhi_VAO_IsValid(const struct rhi_VAO vao) {
  return vao.ID != 0 && vao.ebo_id > 0;
}