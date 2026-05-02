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

/// @brief prepares state for drawing
void rhi_device_begin_frame();

/// @brief pushes shader to global state
void rhi_device_use_program(rhi_Program);

/// @brief binds fbo for drawing
/// @param fbo framebuffer handle
/// (NULL - if you wand to bind initial fbo)
void rhi_device_bind_fbo(rhi_Fbo* fbo);

/// @brief draw call
void rhi_device_draw(rhi_VAO, int count);

/// @brief renders vao instances at the same time
void rhi_device_draw_instanced(rhi_VAO, int count, int instances);

/// @brief get current render stats
/// (vertices, drawcalls and other gpu related stuff)
rhi_RenderStats rhi_get_stats();