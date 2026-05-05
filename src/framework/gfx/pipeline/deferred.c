#include "deferred.h"
#include "framework/app/app.h"

#include <kernel/gfx/rhi/framebuffer.h>

static rhi_Fbo deferred_fbo;

static const rhi_Texture* tex_diffuse = NULL;
static const rhi_Texture* tex_normal  = NULL;
static const rhi_Texture* tex_depth   = NULL;

void drt_init() {
  int* size = app_get_screen_size();
  rhi_fbo_init(&deferred_fbo, size[0], size[1]);

  // diffuse
  rhi_fbo_add_color(
    &deferred_fbo,
    RHI_TEX_FORMAT_RGB16F,
    RHI_TEX_FILTER_NEAREST
  );

  // normal
  rhi_fbo_add_color(
    &deferred_fbo,
    RHI_TEX_FORMAT_RGB16F,
    RHI_TEX_FILTER_NEAREST
  );

  // depth
  rhi_fbo_add_depth(
    &deferred_fbo,
    RHI_TEX_FORMAT_DEPTH24
  );

  rhi_fbo_set_draw_bufs(&deferred_fbo, (uint[2]){0, 1}, 2);

  RFK_ASSERT(
    rhi_fbo_check(&deferred_fbo),
    "deferred fbo not completed"
  );

  tex_diffuse =
    (rhi_Texture*)vec_get_raw(&deferred_fbo.colorTextures, 0);
  tex_normal =
    (rhi_Texture*)vec_get_raw(&deferred_fbo.colorTextures, 1);
  tex_depth = &deferred_fbo.depthTexture;
}

void drt_on_resize(int w, int h) {
  rhi_fbo_resize(&deferred_fbo, w, h);
}

const rhi_Fbo* drt_get_fbo() {
  return &deferred_fbo;
}

void drt_destroy() {
  rhi_fbo_invalidate(&deferred_fbo);
}

DeferredRenderResult drt_results() {
  return (DeferredRenderResult){.Diffuse = tex_diffuse,
                                .Normal  = tex_normal,
                                .Depth   = tex_depth};
}
