#pragma once

#include "vao.h"
#include "program.h"
#include <cglm/cglm.h>

struct rhi_RenderStats {
  int Vertices;
  int DrawCalls;
  int Triangles;
};

/// @brief init
void rhi_Device_Init();

/// @brief save current state
void rhi_PushState();

/// @brief restore saved state
void rhi_PopState();

void rhi_Device_NewFrame();
void rhi_Device_UseProgram(struct rhi_Program);
void rhi_Device_Draw(struct rhi_VAO, int count);
void rhi_Device_DrawInstanced(struct rhi_VAO, int count, int instances);
struct rhi_RenderStats rhi_GetStats();