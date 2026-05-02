#include "render_device.h"

#include <rfklib/log.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// bindings
uint vao_binding = 0u;
uint program_binding = 0u;

// stats
static rhi_RenderStats stats = { 0 };

// --------------------------------------
//       Init and state management
// --------------------------------------

void rhi_device_init() {
  // init renderer
  RFK_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "failed to init context");
}

void rhi_push_state() {

}

void rhi_pop_state() {

}

void rhi_Device_Clear(vec4 color, uint bufferMask) {
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(bufferMask);
}

// --------------------------------------
//            Drawing
// --------------------------------------

bool vaoValidate(rhi_VAO t) {
  if (vao_binding == t.ID) return true;

  if (!rhi_vao_is_valid(t)) return false;

  vao_binding = t.ID;
  glBindVertexArray(vao_binding);
  return true;
}

void rhi_device_begin_frame() {
  // reset stats
  stats = (rhi_RenderStats){ 0 };

  // invalidate bindings
  vao_binding = 0;
  program_binding = 0;
  glBindVertexArray(0);
  glUseProgram(0);
}

void rhi_device_use_program(rhi_Program p) {
  if (p.handle != program_binding) {
    glUseProgram(p.handle);
    program_binding = p.handle;
  }
}

void rhi_device_draw(rhi_VAO t, int count) {
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

void rhi_device_draw_instanced(rhi_VAO t, int count, int instances) {
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

rhi_RenderStats rhi_get_stats() {
  return stats;
}
