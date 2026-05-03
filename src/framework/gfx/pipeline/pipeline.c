#include "pipeline.h"

#include "deferred.h"
#include <kernel/core/window.h>
#include <kernel/gfx/gfx.h>

static rhi_Fbo ping;
static rhi_Fbo pong;

// init rendering pipeline
void rpl_init() {
  int* size = wnd_size();

  // init ping fbo
  rhi_fbo_init(&ping, size[0], size[1]);

  // init pong fbo
  rhi_fbo_init(&pong, size[0], size[1]);
}

void rpl_perform(DeferredRenderResult r) {
}

void rpl_destroy() {
  rhi_fbo_invalidate(&ping);
  rhi_fbo_invalidate(&pong);
}