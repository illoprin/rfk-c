#pragma once

#include "util.h"

struct rhi_Buffer {
  GLuint ID;
  size_t size;
  rhi_BufferType type;
  rhi_BufferUsage usage;
  void* mapping;
};

void rhi_Buf_Init(struct rhi_Buffer*, rhi_BufferType type, rhi_BufferUsage usage);
void rhi_Buf_Allocate(struct rhi_Buffer*, const void* data, size_t size);
void rhi_Buf_Update(struct rhi_Buffer*, size_t offset, const void* data, size_t size);
void rhi_Buf_Clear(struct rhi_Buffer*, size_t size);
void rhi_Buf_Invalidate(struct rhi_Buffer*);