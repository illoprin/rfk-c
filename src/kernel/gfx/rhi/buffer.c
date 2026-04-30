#include "buffer.h"

void rhi_Buf_Init(struct rhi_Buffer* buf, rhi_BufferType type, rhi_BufferUsage usage) {
  if (buf == NULL) return;
  *buf = (struct rhi_Buffer){ 0 };

  buf->type = type;
  buf->usage = usage;

  glGenBuffers(1, &buf->ID);
  glBindBuffer(type, buf->ID);

  LogInfo("buffer [ID = %d, Type = 0x%04x, Usage = 0x%04x] created",
    buf->ID, buf->type, buf->usage);
}

void rhi_Buf_Allocate(struct rhi_Buffer* buf, const void* data, size_t size) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  glBindBuffer(buf->type, buf->ID);
  glBufferData(buf->type, size, data, buf->usage);
  buf->size = size;
}

void rhi_Buf_Update(struct rhi_Buffer* buf, size_t offset, const void* data, size_t size) {
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

  glBindBuffer(buf->type, buf->ID);
  glBufferSubData(buf->type, offset, size, data);
}

void rhi_Buf_Clear(struct rhi_Buffer* buf, size_t size) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  rhi_Buf_Allocate(buf, NULL, size);
}

void rhi_Buf_Invalidate(struct rhi_Buffer* buf) {
  if (buf == NULL) return;
  if (buf->ID == 0) return;

  glDeleteBuffers(1, &buf->ID);

  LogInfo("buffer [ID = %d, Type = 0x%04x, Usage = 0x%04x] deleted",
    buf->ID, buf->type, buf->usage);
  *buf = (struct rhi_Buffer){ 0 };
}
