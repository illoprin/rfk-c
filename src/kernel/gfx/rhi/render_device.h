#pragma once

#include "vao.h"

struct rhi_RenderStats {
  uint Vertices;
  uint DrawCalls;
  uint Triangles;
};

void rhi_RenderDevice_NewFrame();
void rhi_RenderDevice_Draw(struct rhi_VAO, uint count);
void rhi_RenderDevice_DrawInstanced(struct rhi_VAO, uint count, uint instances);
void rhi_RenderDevice_SetupErrorCallback();
struct rhi_RenderStats rhi_GetStats();