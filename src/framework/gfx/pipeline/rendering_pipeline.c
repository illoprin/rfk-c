#include "rendering_pipeline.h"

#include "deferred.h"
#include "post_pipeline.h"
#include <framework/app/app.h>
#include <kernel/core/window.h>
#include <kernel/gfx/gfx.h>

// ~512 bytes
static uch active_post_effects = 0u;

// init rendering pipeline
void rpl_init() {

  // init deferred framebuffer
  drt_init();

  // init post processing pipeline
  ppl_init();
}

void rpl_render(void (*renderFunc)()) {

  int*     size = app_get_screen_size();
  rhi_Fbo* drt  = drt_get_fbo();

  // geometry pass
  rhi_device_bind_fbo(drt);
  if (renderFunc) renderFunc();
  rhi_Fbo* result = NULL;

  // geometry screen space effects

  // lighting pass and effects

  // filters (finalize frame)

  // blit to screen
  rhi_device_blit_to_screen(result, size[0], size[1]);
}

void rpl_resize(int w, int h) {
}

void rpl_destroy() {
  ppl_destroy();
  drt_destroy();
}