#ifndef RFK_COLOR_GRADING_H
#define RFK_COLOR_GRADING_H

#include "framework/app/initial_assets.h"
#include "framework/gfx/post_effects/post_effects.h"
#include "framework/gfx/resource/program_loader.h"
#include "kernel/core/files.h"
#include "kernel/gfx/rhi/program.h"
#include "kernel/gfx/rhi/render_device.h"
#include "kernel/gfx/rhi/util.h"

// PostEffects (pe) module
// Color Grading sub-module

// @brief get post effect descriptor
// @returns Post Effect struct
PostEffect pe_cg_descriptor();

// @brief init color grading post effect
void pe_cg_init();

// @brief destroy color grading post effect
void pe_cg_destroy();

#endif  // RFK_COLOR_GRADING_H

#define RFK_POST_EFFECTS_IMPL
#ifdef RFK_POST_EFFECTS_IMPL

static rhi_Program cg_prog;

static bool cg_initialized = false;

void pe_cg_init() {
  // init program
  RFK_ASSERT(
    rhi_prog_quick_load(
      &cg_prog,
      FLS_SHADER_PATH("post/screen.vert"),
      FLS_SHADER_PATH("post/color_grading.frag")
    ),
    "failed to load color grading program"
  );

  cg_initialized = true;
}

void pe_cg_destroy() {
  rhi_prog_invalidate(&cg_prog);
}

void pe_cg_render_pass(PostProcessingContext ctx) {
  if (!cg_initialized) return;
  rhi_device_bind_fbo(ctx.OutFbo);
  rhi_device_clear(RHI_COLOR_BIT);
  rhi_device_use_program(cg_prog);

  // TODO: send config

  Mesh* quad = ia_mesh_quad();
  rhi_device_draw(quad->VAO, quad->indexNum);
}

PostEffect pe_cg_descriptor() {
  return (PostEffect){
    .Active      = true,
    .Initialized = cg_initialized,
    .Name        = "color_grading",
    .RenderPass  = pe_cg_render_pass,
  };
}

#endif  // RFK_POST_EFFECTS_IMPL