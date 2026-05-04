#pragma once

#include "kernel/gfx/rhi/framebuffer.h"
#include "kernel/gfx/rhi/texture.h"

// Deferred Render Target (drt) module

typedef struct {
  const rhi_Texture* Diffuse;
  const rhi_Texture* Normal;
  const rhi_Texture* Depth;
} DeferredRenderResult;

void drt_init();

const rhi_Fbo* drt_get_fbo();

DeferredRenderResult drt_results();

void drt_on_resize(int w, int h);

void drt_destroy();
