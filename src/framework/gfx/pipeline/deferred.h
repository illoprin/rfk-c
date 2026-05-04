#pragma once

// Deferred Render Target (drt) module

typedef struct {
  const rhi_Texture* Diffuse;
  const rhi_Texture* Normal;
  const rhi_Texture* Depth;
} DeferredRenderResult;

void drt_init();

const rhi_Fbo* drt_get_fbo();

DeferredRenderResult drt_results();

void drt_destroy();
