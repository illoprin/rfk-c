#pragma once

#include "util.h"

typedef struct {
  GLuint          ID;
  size_t          size;
  rhi_BufferType  type;
  rhi_BufferUsage usage;
  void*           mapping;
} rhi_Buffer;

void rhi_buf_init(
  rhi_Buffer*, rhi_BufferType type, rhi_BufferUsage usage
);
void rhi_buf_allocate(rhi_Buffer*, const void* data, size_t size);
void rhi_buf_update(
  rhi_Buffer*, size_t offset, const void* data, size_t size
);
void rhi_buf_clear(rhi_Buffer*, size_t size);
void rhi_buf_invalidate(rhi_Buffer*);