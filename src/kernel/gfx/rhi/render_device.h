#pragma once

#include "framebuffer.h"
#include "program.h"
#include "texture.h"
#include "vao.h"
#include <cglm/cglm.h>

typedef struct {
  int Vertices;
  int DrawCalls;
  int Triangles;
} rhi_RenderStats;

typedef enum {
  RHI_COLOR_BIT   = GL_COLOR_BUFFER_BIT,
  RHI_DEPTH_BIT   = GL_DEPTH_BUFFER_BIT,
  RHI_STENCIL_BIT = GL_STENCIL_BUFFER_BIT,
} rhi_ClearBuffers;

/// @brief init
void rhi_device_init();

/// @brief clear buffers
void rhi_device_clear(vec4 color, uint bufferMask);

/// @brief transfer bits from one framebuffer to another
/// @param src source framebuffer id
/// @param dst destination framebuffer id
/// @param w0 width of source blitting area
/// @param h0 height of source blitting area
/// @param w1 width of destination blitting area
/// @param h1 height of destination blitting area
/// @param mask what we will blit (RHI_COLOR_BIT / RHI_DEPTH_BIT)
/// @param src_ca source color attachment index
/// @param src_ca destination color attachment index
void rhi_device_blit(
  uint src, uint dst, uint w0, uint h0, uint w1, uint h1, uint mask,
  uint src_ca, uint dst_ca
);

/// @brief blit fbo to initial fbo
void rhi_device_blit_to_screen(rhi_Fbo* src, uint sw, uint sh);

/// @brief get graphics card name string ptr
/// @return graphics card name string ptr
const char* rhi_device_str_renderer();

/// @brief get api version string ptr
/// @return get api version string ptr
const char* rhi_device_str_version();

/// @brief save current state
void rhi_push_state();

/// @brief restore saved state
void rhi_pop_state();

/// @brief prepares state for drawing
void rhi_device_begin_frame();

/// @brief pushes shader to global state
void rhi_device_use_program(rhi_Program);

/// @brief binds texture into sampler unit
/// @param tex target texture object
/// @param unit index of sampler unit
void rhi_device_bind_tex(rhi_Texture tex, int unit);

/// @brief binds fbo for drawing
/// @param fbo framebuffer handle
/// (NULL - if you wand to bind initial fbo)
void rhi_device_bind_fbo(rhi_Fbo* fbo);

/// @brief draw call
void rhi_device_draw(rhi_VAO, int count);

/// @brief renders vao instances at the same time
void rhi_device_draw_instanced(rhi_VAO, int count, int instances);

// ------------------------------
//        OpenGL setters
// ------------------------------

/// @brief get current render stats
/// (vertices, drawcalls and other gpu related stuff)
rhi_RenderStats rhi_get_stats();