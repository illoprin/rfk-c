#include "rendering_pipeline.h"

#include "deferred.h"
#include "framework/config.h"
#include <framework/app/app.h>
#include <framework/gfx/post_effects/post_effects.h>
#include <kernel/core/window.h>
#include <kernel/gfx/gfx.h>

// ~512 bytes
static uch      active_post_effects = 0u;
static rhi_Fbo* drt_fbo             = NULL;
static DeferredRenderResult drt_res = { 0 };

static Camera* active_camera = NULL;

// init rendering pipeline
void rpl_init() {

  // init deferred framebuffer
  drt_init();
  drt_fbo = drt_get_fbo();

  // init post processing pipeline
  post_effects_init();
}

void rpl_render(RplRenderFunc renderFunc) {
  int* size = app_get_screen_size();

  // geometry pass
  rhi_device_bind_fbo(drt_fbo);
  if (renderFunc) renderFunc();

  // apply post effects
  rhi_Fbo* result = post_effects_perform(active_camera);

  // blit to screen
  rhi_device_blit_to_screen(result, size[0], size[1]);
}

void rpl_resize(int w, int h) {
  int* screen_size = app_get_screen_size();
  rhi_fbo_resize(drt_fbo, screen_size[0], screen_size[1]);
}

void rpl_push_camera(Camera* c) {
  active_camera = c;
}

void rpl_destroy() {
  post_effects_destroy();
  drt_destroy();
}