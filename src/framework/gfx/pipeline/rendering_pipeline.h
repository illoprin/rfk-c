
#pragma once

#include "deferred.h"
#include <stdbool.h>
#include "framework/gfx/camera.h"

typedef void (*RplRenderFunc)();

// Rendering Pipeline (rpl) module
/// @brief init post pipeline resources
/// call it after initializing window system
void rpl_init();

/// @brief do post processing
/// @param f geometry render pass func
void rpl_render(RplRenderFunc f);

/// @brief set active camera (uses for post effects)
void rpl_push_camera(Camera*);

/// @brief call it on window resizes
void rpl_resize(int w, int h);

/// @brief free resources and shutdown
void rpl_destroy();
