#pragma once

#include "framework/gfx/camera.h"
#include "framework/gfx/pipeline/deferred.h"
#include "kernel/gfx/rhi/framebuffer.h"

// 40 bytes
typedef struct {
  const rhi_Texture* InColor;
  const rhi_Fbo*     OutFbo;
  const rhi_Texture* InNormal;
  const rhi_Texture* InDepth;
  const Camera*      Camera;
} PostProcessingContext;

typedef void (*PostEffectInitFunc)();
typedef void (*PostEffectRenderFunc)(PostProcessingContext);

// 26 bytes (32 bytes)
typedef struct {
  char                 Name[16];
  PostEffectRenderFunc RenderPass;
  bool                 Active;
  bool                 Initialized;
} PostEffect;

void           post_effects_init();
const rhi_Fbo* post_effects_perform(Camera* c);
void           post_effects_destroy();