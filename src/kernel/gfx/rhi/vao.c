#include "vao.h"

void rhi_vao_init(rhi_VAO* o) {
  if (o == NULL) return;
  *o = (rhi_VAO){ 0 };

  glGenVertexArrays(1, &o->ID);
  glBindVertexArray(o->ID);
  glBindVertexArray(0);
  LogInfo("vao [ID = %d] created", o->ID);
}

void rhi_vao_add_index_buf(rhi_VAO* o, rhi_Buffer buf) {
  if (o == NULL) return;
  if (o->ID == 0) return;

  if (buf.type != RHI_BUF_INDEX) {
    LogErr("vao [ID = %d] AddIndexBuffer - you cannot assign non index buffer");
    return;
  }

  glBindVertexArray(o->ID);
  glBindBuffer(buf.type, buf.ID);
  glBindVertexArray(0);
  o->eboId = buf.ID;
}

void rhi_vao_add_attrs(rhi_VAO* o, rhi_Buffer buf, rhi_Attribute* attrs, int numAttrs) {
  if (o == NULL) return;
  if (o->ID == 0) return;

  if (buf.type != RHI_BUF_VERTEX) {
    LogErr("vao [ID = %d] AddVertexAttribute - you cannot assign non vertex buffer");
    return;
  }

  glBindVertexArray(o->ID);
  glBindBuffer(GL_ARRAY_BUFFER, buf.ID);

  for (int i = 0; i < numAttrs; i++) {
    rhi_Attribute attr = *(attrs + i);

    glEnableVertexAttribArray(attr.Location);
    if (attr.Type == RHI_FLOAT) {
      glVertexAttribPointer(
        attr.Location,
        attr.Components,
        attr.Type,
        GL_FALSE,
        attr.Stride,
        (void*)attr.Offset
      );
    } else {
      glVertexAttribIPointer(
        attr.Location,
        attr.Components,
        attr.Type,
        attr.Stride,
        (void*)attr.Offset
      );
    }
    glVertexAttribDivisor(attr.Location, attr.Divisor);
  }
  glBindVertexArray(0);

}

bool rhi_vao_is_valid(rhi_VAO o) {
  return o.eboId != 0 && o.ID != 0;
}

void rhi_vao_invalidate(rhi_VAO* o) {
  if (o == NULL) return;
  if (o->ID == 0) return;
  glDeleteVertexArrays(1, &o->ID);
  LogInfo("vao [ID = %d] deleted", o->ID);
  *o = (rhi_VAO){ 0 };
}
