#include "buffer.h"

void rhi_buf_init(rhi_Buffer* buf, rhi_BufferType type, rhi_BufferUsage usage) {
  if (buf == NULL) return;
  *buf = (rhi_Buffer){ 0 };

  buf->type = type;
  buf->usage = usage;

  glBindVertexArray(0);
  glGenBuffers(1, &buf->ID);
  glBindBuffer(type, buf->ID);

  LogInfo("buffer [ID = %d, Type = 0x%04x, Usage = 0x%04x] created",
    buf->ID, buf->type, buf->usage);
}

void rhi_buf_allocate(rhi_Buffer* buf, const void* data, size_t size) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  glBindVertexArray(0);
  glBindBuffer(buf->type, buf->ID);
  glBufferData(buf->type, size, data, buf->usage);
  buf->size = size;
}

void rhi_buf_update(rhi_Buffer* buf, size_t offset, const void* data, size_t size) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;
  if (buf->usage != RHI_USAGE_DYNAMIC || buf->usage != RHI_USAGE_STREAM) {
    LogWarn("buf [ID = %d] can't update buffer with NON dynamic usage", buf->ID);
    return;
  }
  if (buf->size < size + offset) {
    LogWarn("buffer [ID = %d] can't update more than allocated", buf->ID);
    return;
  }

  glBindVertexArray(0);
  glBindBuffer(buf->type, buf->ID);
  glBufferSubData(buf->type, offset, size, data);
}

void rhi_buf_clear(rhi_Buffer* buf, size_t size) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  rhi_buf_allocate(buf, NULL, size);
}

void rhi_buf_invalidate(rhi_Buffer* buf) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  glDeleteBuffers(1, &buf->ID);

  LogInfo("buffer [ID = %d, Type = 0x%04x, Usage = 0x%04x] deleted",
    buf->ID, buf->type, buf->usage);
  *buf = (rhi_Buffer){ 0 };
}
