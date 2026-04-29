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

// --- Format ---
typedef enum : uint {
  RHI_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  RHI_UNSIGNED_INT = GL_UNSIGNED_INT,
  RHI_INTEGER = GL_INT,
  RHI_FLOAT = GL_FLOAT,
} rhi_DataType;

// storage_buffer_ARB
struct rhi_Buffer {
  uint ID;
  size_t size;
  rhi_BufferType type;
  rhi_BufferUsage usage;
};

// --- API ---
void rhi_Buffer_Create(
  struct rhi_Buffer* buf,
  size_t size,
  const void* data,
  rhi_BufferType usage,
  rhi_BufferUsage lifetime
);
void rhi_Buffer_Update(
  struct rhi_Buffer* buf,
  size_t offset,
  size_t size,
  const void* data
);
void rhi_Buffer_Bind(struct rhi_Buffer* buf);
void rhi_Buffer_BindBase(struct rhi_Buffer* buf, uint32_t slot);
void rhi_Buffer_Invalidate(struct rhi_Buffer* buf);