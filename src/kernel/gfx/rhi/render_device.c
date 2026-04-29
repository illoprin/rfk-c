#include "render_device.h"

#include <kernel/core/log.h>

uint _currentBinding = 0u;
static struct rhi_RenderStats stats = { 0 };

bool vaoValidate(struct rhi_VAO t) {

  if (_currentBinding == t.ID) {
    return true;
  }

  if (!rhi_VAO_IsValid(t)) {
    return false;
  }

  _currentBinding = t.ID;
  glBindVertexArray(_currentBinding);
  return true;
}

void rhi_RenderDevice_NewFrame() {
  // reset stats
  stats = (struct rhi_RenderStats){ 0 };

  // invalidate bindings
  _currentBinding = 0;
  glBindVertexArray(0);
}

void rhi_RenderDevice_Draw(struct rhi_VAO t, uint count) {
  // validate vao
  if (!vaoValidate(t)) {
    return;
  }

  // draw call
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

  // update stats
  stats.Vertices += count;
  stats.Triangles += count / 3;
  stats.DrawCalls++;
}

void rhi_RenderDevice_DrawInstanced(struct rhi_VAO t, uint count, uint instances) {
  // validate vao
  if (!vaoValidate(t)) {
    return;
  }

  // draw call
  glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, instances);

  // update stats
  stats.Vertices += (count)*instances;
  stats.Triangles += (count / 3) * instances;
  stats.DrawCalls++;
}

struct rhi_RenderStats rhi_GetStats() {
  return stats;
}
