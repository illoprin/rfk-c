#pragma once

#include "vao.h"
#include "program.h"
#include <cglm/cglm.h>

typedef struct {
  int Vertices;
  int DrawCalls;
  int Triangles;
} rhi_RenderStats;

/// @brief init
void rhi_device_init();

/// @brief save current state
void rhi_push_state();

/// @brief restore saved state
void rhi_pop_state();

void rhi_device_begin_frame();
void rhi_device_use_program(rhi_Program);
void rhi_device_draw(rhi_VAO, int count);
void rhi_device_draw_instanced(rhi_VAO, int count, int instances);
rhi_RenderStats rhi_get_stats();