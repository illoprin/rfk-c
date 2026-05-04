#pragma once

#include <kernel/gfx/rhi/texture.h>

typedef void (*PostEffectInitFunc)();
typedef rhi_Texture* (*PostEffectRenderFunc)(
  const rhi_Texture* in_Color, const rhi_Texture* in_Normal,
  const rhi_Texture* in_Depth
);

// 26 bytes (32 bytes)
typedef struct {
  char                 Name[16];
  PostEffectRenderFunc RenderPass;
  bool                 Active;
  bool                 Initialized;
} PostEffect;

/// @brief init post processing pipeline resources
void ppl_init();

/// @brief perform post effects
/// uses fbo ping-pong technique for post processing
/// @param v post effects array
void ppl_set_post_effects(const PostEffect*);

void ppl_destroy();