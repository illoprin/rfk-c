#include "post_pipeline.h"

#include <framework/app/app.h>
#include <framework/app/initial_assets.h>
#include <kernel/gfx/rhi/framebuffer.h>

static rhi_Fbo g_ping_pong_fbos[2];

static const PostEffect* g_effects;

void ppl_init() {

  int* size = app_get_screen_size();

  // init ping pong fbos
  for (uch i = 0; i < 2; ++i)
    rhi_fbo_init(&g_ping_pong_fbos[i], size[0], size[1]);
}

void ppl_set_post_effects(const PostEffect* array) {
  if (!array) return;
  g_effects = array;
}

void ppl_perform() {
  int* size = app_get_screen_size();
}

void ppl_destroy() {
  // invalidate fbos
  for (uch i = 0; i < 2; ++i)
    rhi_fbo_invalidate(&g_ping_pong_fbos[i]);
}
