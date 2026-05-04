#pragma once

#include <cglm/cglm.h>

// 32 bytes
typedef struct {
  vec4 Position;  // w - radius
  vec4 Color;     // w - intensity
} PointLight;