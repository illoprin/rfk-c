#include "buffer.h"

#include <string.h>
#include <kernel/core/log.h>

static GLenum getGLUsage(rhi_BufferUsage usage) {
  switch (usage) {
    case RHI_USAGE_STATIC:  return 0;
    case RHI_USAGE_DYNAMIC: return GL_DYNAMIC_STORAGE_BIT;
  }
  return GL_STATIC_DRAW;
}

// --- Create ---

void rhi_Buffer_Create(
  rhi_Buffer* buf,
  size_t size,
  const void* data,
  rhi_BufferType type,
  rhi_BufferUsage usage
) {
  RFK_ASSERT(buf && size > 0);

  glCreateBuffers(1, &buf->ID);

  buf->size = size;
  buf->type = type;
  buf->usage = usage;

  glNamedBufferStorage(
    buf->ID,
    size,
    data,
    getGLUsage(usage)
  );

  LogInfo("buffer created [ID: %u, size: %zu]", buf->ID, size);
}

// --- Update (main path) ---

void rhi_Buffer_Update(
  rhi_Buffer* buf,
  size_t offset,
  size_t size,
  const void* data
) {
  RFK_ASSERT(buf && buf->ID);
  RFK_ASSERT(offset + size <= buf->size);
  RFK_ASSERT(buf->type >= RHI_USAGE_DYNAMIC);

  // update classical way
  glNamedBufferSubData(buf->ID, offset, size, data);
}

// --- Bind (vertex/index/etc) ---

void rhi_Buffer_Bind(rhi_Buffer* buf) {
  RFK_ASSERT(buf && buf->ID);
  glBindBuffer(buf->type, buf->ID);
}

// --- BindBase (UBO/SSBO) ---

void rhi_Buffer_BindBase(rhi_Buffer* buf, uint32_t slot) {
  RFK_ASSERT(buf && buf->ID);

  if (buf->type == RHI_BUFFER_STORAGE ||
    buf->type == RHI_BUFFER_UNIFORM) {
    glBindBufferBase(buf->type, slot, buf->ID);
  }
}

// --- Destroy ---

void rhi_Buffer_Invalidate(rhi_Buffer* buf) {
  if (!buf || buf->ID == 0)
    return;

  glDeleteBuffers(1, &buf->ID);
  LogInfo("buffer [ID = %d] deleted", buf->ID);

  buf->ID = 0;
  buf->size = 0;
}