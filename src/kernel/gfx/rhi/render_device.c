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

void APIENTRY messageCallback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam) {
  LogErr("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
    type, severity, message);
}

void rhi_RenderDevice_SetupErrorCallback() {
  // Enable debug output
  glEnable(GL_DEBUG_OUTPUT);

  // Optional: Enable synchronous output for easier debugging (guarantees the 
  // callback is called in the same thread as the error-causing GL command)
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  glDebugMessageCallback(messageCallback, 0);
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
  glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, 1);

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
