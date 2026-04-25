#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel/core/types.h>
#include <glad/glad.h>

// --- Buffer roles (usage semantics) ---
typedef enum {
  RHI_BUFFER_VERTEX = GL_ARRAY_BUFFER,
  RHI_BUFFER_INDEX = GL_ELEMENT_ARRAY_BUFFER,
  RHI_BUFFER_UNIFORM = GL_UNIFORM_BUFFER,
  RHI_BUFFER_STORAGE = GL_SHADER_STORAGE_BUFFER,
  RHI_BUFFER_INDIRECT = GL_DRAW_INDIRECT_BUFFER
} rhi_BufferType;

// --- Update model ---
typedef enum {
  RHI_USAGE_STATIC, // GPU read only
  RHI_USAGE_DYNAMIC, // CPU -> GPU (SubData)
  RHI_USAGE_STREAM   // Persistent Mapping (Coherent)
} rhi_BufferUsage;

// storage_buffer_arb
typedef struct {
  uint ID;
  size_t size;
  rhi_BufferType type;
  rhi_BufferUsage usage;
} rhi_Buffer;

// --- API ---
void rhi_Buffer_Create(
  rhi_Buffer* buf,
  size_t size,
  const void* data,
  rhi_BufferType usage,
  rhi_BufferUsage lifetime
);

void rhi_Buffer_Update(
  rhi_Buffer* buf,
  size_t offset,
  size_t size,
  const void* data
);

void rhi_Buffer_Bind(rhi_Buffer* buf);
void rhi_Buffer_BindBase(rhi_Buffer* buf, uint32_t slot);
void rhi_Buffer_Invalidate(rhi_Buffer* buf);