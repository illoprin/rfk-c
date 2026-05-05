#ifndef RFK_COLOR_GRADING_H
#define RFK_COLOR_GRADING_H

#include "framework/app/initial_assets.h"
#include "framework/gfx/post_effects/post_effects.h"
#include "framework/gfx/resource/program_loader.h"
#include "kernel/core/files.h"
#include "kernel/gfx/rhi/program.h"
#include "kernel/gfx/rhi/render_device.h"
#include "kernel/gfx/rhi/util.h"
#include <cglm/cglm.h>

struct ColorGradingConfig {
  bool Use;

  // Basic adjustments 
  float Contrast;
  float Saturation;
  float Brightness;

  // Color balance
  vec3 ShadowsColor;
  vec3 MidColor;
  vec3 HighlightColor;
  float ColorStrength;
};

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

static struct ColorGradingConfig conf = {
  .Brightness = 1.57,
  .Contrast = 2.28,
  .Saturation = 0.85,
  .ShadowsColor = {0.16, 0.18, 0.3},
  .MidColor = {0.68, 0.518, 0.33},
  .HighlightColor = {0.938, 0.641, 0.438},
};

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

  // send color
  rhi_device_bind_tex(ctx.InColor, 0);
  rhi_prog_uniform_1i(cg_prog, "u_color", 0);

  // set uniforms
  rhi_prog_uniform_1f(cg_prog, "u_brightness", conf.Brightness);
  rhi_prog_uniform_1f(cg_prog, "u_contrast", conf.Contrast);
  rhi_prog_uniform_1f(cg_prog, "u_saturation", conf.Saturation);

  rhi_prog_uniform_3f(cg_prog, "u_shadow_color", conf.ShadowsColor);
  rhi_prog_uniform_3f(cg_prog, "u_mid_color", conf.MidColor);
  rhi_prog_uniform_3f(cg_prog, "u_highlight_color", conf.HighlightColor);
  rhi_prog_uniform_1f(cg_prog, "u_color_strength", conf.ColorStrength);

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