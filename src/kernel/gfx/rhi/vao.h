#pragma once

#include "util.h"
#include "buffer.h"

// Attribute description
typedef struct {
  int         Location;
  int         Components;
  rhi_DataType Type;
  size_t       Offset;
  size_t       Stride;
  int         Divisor;
} rhi_Attribute;

typedef struct {
  GLuint ID;
  GLuint eboId;
} rhi_VAO;

void rhi_vao_init(rhi_VAO*);
void rhi_vao_add_index_buf(rhi_VAO*, rhi_Buffer);
void rhi_vao_add_attrs(rhi_VAO*, rhi_Buffer, rhi_Attribute*, int numAttrs);
bool rhi_vao_is_valid(rhi_VAO);
void rhi_vao_invalidate(rhi_VAO*);