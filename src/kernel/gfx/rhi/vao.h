#pragma once

#include "util.h"
#include "buffer.h"

// Attribute description
struct rhi_Attribute {
  int         Location;
  int         Components;
  rhi_DataType Type;
  size_t       Offset;
  size_t       Stride;
  int         Divisor;
};

struct rhi_VAO {
  GLuint ID;
  GLuint eboId;
};

void rhi_VAO_Init(struct rhi_VAO*);
void rhi_VAO_AddIndexBuffer(struct rhi_VAO*, struct rhi_Buffer);
void rhi_VAO_AddAttributes(struct rhi_VAO*, struct rhi_Buffer, struct rhi_Attribute*, int numAttrs);
void rhi_VAO_Bind(struct rhi_VAO*);
bool rhi_VAO_IsValid(struct rhi_VAO);
void rhi_VAO_Invalidate(struct rhi_VAO*);