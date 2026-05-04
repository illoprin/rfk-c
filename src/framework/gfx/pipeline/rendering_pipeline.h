
#pragma once

#include "deferred.h"
#include <stdbool.h>

// Rendering Pipeline (rpl) module
/// @brief init post pipeline resources
/// call it after initializing window system
void rpl_init();

/// @brief do post processing
void rpl_render();

/// @brief call it on window resizes
void rpl_resize(int w, int h);

/// @brief free resources and shutdown
void rpl_destroy();
