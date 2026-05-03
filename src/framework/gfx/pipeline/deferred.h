#pragma once

/**
 *
 * --------------------------------
 * *   Deferred Render Target     *
 * --------------------------------
 *
 *
 */

typedef struct {
  const rhi_Texture* Diffuse;
  const rhi_Texture* Normal;
  const rhi_Texture* Depth;
} DeferredRenderResult;

void drt_init();

void drt_begin_frame();

DeferredRenderResult drt_results();

void drt_destroy();