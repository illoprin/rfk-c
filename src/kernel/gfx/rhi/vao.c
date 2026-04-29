#include "vao.h"

void rhi_VAO_Init(struct rhi_VAO* o) {
  if (o == NULL) return;
  *o = (struct rhi_VAO){ 0 };

  glGenVertexArrays(1, &o->ID);
  glBindVertexArray(o->ID);
  LogInfo("vao [ID = %d] created", o->ID);
}

void rhi_VAO_AddIndexBuffer(struct rhi_VAO* o, struct rhi_Buffer buf) {
  if (o == NULL) return;
  if (o->ID == 0) return;

  if (buf.type != RHI_BUF_INDEX) {
    LogErr("vao [ID = %d] AddIndexBuffer - you cannot assign non index buffer");
    return;
  }

  glBindVertexArray(o->ID);
  glBindBuffer(buf.type, buf.ID);
  o->eboId = buf.ID;
}

void rhi_VAO_AddAttributes(struct rhi_VAO* o, struct rhi_Buffer buf, struct rhi_Attribute* attrs, int numAttrs) {
  if (o == NULL) return;
  if (o->ID == 0) return;

  if (buf.type != RHI_BUF_VERTEX) {
    LogErr("vao [ID = %d] AddVertexAttribute - you cannot assign non vertex buffer");
    return;
  }

  glBindVertexArray(o->ID);
  glBindBuffer(GL_ARRAY_BUFFER, buf.ID);

  for (int i = 0; i < numAttrs; i++) {
    struct rhi_Attribute attr = *(attrs + i);

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
    }
    else {
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

}

void rhi_VAO_Bind(struct rhi_VAO* o) {
  if (o == NULL) return;
  if (o->ID == 0) return;
  glBindVertexArray(o->ID);
}

bool rhi_VAO_IsValid(struct rhi_VAO o) {
  return o.eboId != 0 && o.ID != 0;
}

void rhi_VAO_Invalidate(struct rhi_VAO* o) {
  if (o == NULL) return;
  if (o->ID == 0) return;
  glDeleteVertexArrays(1, &o->ID);
  LogInfo("vao [ID = %d] deleted", o->ID);
  *o = (struct rhi_VAO){ 0 };
}
