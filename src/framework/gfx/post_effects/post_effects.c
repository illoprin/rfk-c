#include "post_effects.h"
#include "framework/app/app.h"
#include "framework/gfx/pipeline/deferred.h"
#include "kernel/gfx/rhi/framebuffer.h"
#include "kernel/gfx/rhi/texture.h"
#include "kernel/gfx/rhi/util.h"

#include <cglm/cglm.h>
#include <framework/config.h>
#include <framework/gfx/post_effects/color_grading.h>

static vec3                  samples[MAX_SAMPLES];
static rhi_Fbo               g_ping_pong[2]                     = {0};
static uint                  write_buf                          = 0;
static uint                  read_buf                           = 1;
static PostEffect            post_effects[MAX_EFFECTS_PER_PASS] = {0};
static uch                   post_effects_count;
static PostProcessingContext ctx;

void post_effects_init() {
  // init ping pong fbos
  int* size = app_get_screen_size();
  for (uch i = 0; i < 2; ++i) {
    rhi_fbo_init(&g_ping_pong[i], size[0], size[1]);
    rhi_fbo_add_color(
      &g_ping_pong[i],
      RHI_TEX_FORMAT_RGB16F,
      RHI_TEX_FILTER_NEAREST
    );
    RFK_ASSERT(rhi_fbo_check(&g_ping_pong[i]), "invalid fbo");
  }

  // init effects shared resources
  // for(uch i = 0; i < MAX_SAMPLES; ++i)

  // init effects
  pe_cg_init();

  // create post effects descriptors array
  post_effects[0]    = pe_cg_descriptor();  // color grading
  post_effects_count = 1;
}

const rhi_Fbo* post_effects_perform(Camera* c) {
  // get deferred render result
  DeferredRenderResult res = drt_results();

  // setup context
  ctx.Camera   = c;
  ctx.InColor  = res.Diffuse;
  ctx.InDepth  = res.Depth;
  ctx.InNormal = res.Normal;
  ctx.OutFbo   = NULL;

  // perform effects
  for (uch i = 0; i < post_effects_count; ++i) {
    // swap read and write buf indices
    uch tmp   = read_buf;
    read_buf  = write_buf;
    write_buf = tmp;

    // get in and out fbos pointers
    rhi_Fbo* in_fbo  = &g_ping_pong[read_buf];
    rhi_Fbo* out_fbo = &g_ping_pong[write_buf];

    // get reading texture
    const rhi_Texture* in_tex;
    in_tex = i == 0 ? res.Diffuse
                    : (rhi_Texture*)vec_get_raw(&in_fbo->colorTextures, 0);

    ctx.InColor = in_tex;
    ctx.OutFbo  = out_fbo;

    // perform effect
    post_effects[i].RenderPass(ctx);
  }
  return ctx.OutFbo;
}

void post_effects_destroy() {
  pe_cg_destroy();
  for (uch i = 0; i < 2; ++i) {
    rhi_fbo_invalidate(&g_ping_pong[i]);
  }
}
