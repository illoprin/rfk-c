#include "render_device.h"

#include "framebuffer.h"
#include <kernel/core/defs.h>
#include <rfklib/log.h>
#include <string.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// bindings
static uint vao_binding     = 0u;
static uint program_binding = 0u;
static uint fbo_binding     = 0u;

// misc
static const char* g_renderer;
static const char* g_api_version;

// stats
static rhi_RenderStats stats = {0};

// --------------------------------------
//       Init and state management
// --------------------------------------

void rhi_device_init() {
  // init renderer
  RFK_ASSERT(
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
    "failed to init context"
  );

  g_renderer    = glGetString(GL_RENDERER);
  g_api_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
  glClearColor(0.f, 0.f, 0.f, 0.f);
}

const char* rhi_device_str_renderer() {
  return g_renderer;
}

const char* rhi_device_str_version() {
  return g_api_version;
}

void rhi_push_state() {
}

void rhi_pop_state() {
}

void rhi_device_clear(uint bufferMask) {
  glClear(bufferMask);
}

void rhi_device_blit(
  uint src, uint dst, uint w0, uint h0, uint w1, uint h1, uint mask,
  uint src_ca
) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, src);
  glReadBuffer(GL_COLOR_ATTACHMENT0 + src_ca);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst);
  glBlitFramebuffer(0, 0, w0, h0, 0, 0, w1, h1, mask, GL_NEAREST);
}

void rhi_device_blit_to_screen(const rhi_Fbo* src, uint sw, uint sh) {
  if (!src) return;

  uint src_id = src->ID;
  uint src_w  = src->width;
  uint src_h  = src->height;

  if (src_id == 0) return;

  rhi_device_blit(src_id, 0, src_w, src_h, sw, sh, RHI_COLOR_BIT, 0);
}

// --------------------------------------
//            Drawing
// --------------------------------------

void rhi_device_bind_fbo(const rhi_Fbo* fbo) {
  uint id;
  if (fbo == NULL || fbo->ID == 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    id = 0;
  } else {
    if (fbo->ID == fbo_binding) { return; }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
    glViewport(0, 0, fbo->width, fbo->height);
    id = fbo->ID;
  }

  fbo_binding = id;
}

bool vaoValidate(rhi_VAO t) {
  if (vao_binding == t.ID) { return true; }

  if (!rhi_vao_is_valid(t)) { return false; }

  vao_binding = t.ID;
  glBindVertexArray(vao_binding);
  return true;
}

void rhi_device_begin_frame() {
  // reset stats
  stats = (rhi_RenderStats){0};

  // invalidate bindings
  vao_binding     = 0;
  program_binding = 0;
  fbo_binding     = 0;
  glBindVertexArray(0);
  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rhi_device_use_program(rhi_Program p) {
  if (p.handle == program_binding) { return; }
  glUseProgram(p.handle);
  program_binding = p.handle;
}

void rhi_device_bind_tex(rhi_Texture tex, int unit) {
  if (tex.ID == 0) { return; }
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(tex.type, tex.ID);
}

void rhi_device_draw(rhi_VAO t, int count) {
  // validate vao
  if (!vaoValidate(t)) { return; }

  // draw call
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

  // update stats
  stats.Vertices  += count;
  stats.Triangles += count / 3;
  stats.DrawCalls++;
}

void rhi_device_draw_instanced(rhi_VAO t, int count, int instances) {
  // validate vao
  if (!vaoValidate(t)) { return; }

  // draw call
  glDrawElementsInstanced(
    GL_TRIANGLES,
    count,
    GL_UNSIGNED_INT,
    NULL,
    instances
  );

  // update stats
  stats.Vertices  += (count)*instances;
  stats.Triangles += (count / 3) * instances;
  stats.DrawCalls++;
}

rhi_RenderStats rhi_get_stats() {
  return stats;
}
