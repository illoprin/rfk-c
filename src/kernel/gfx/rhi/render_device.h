#pragma once

#include "vao.h"
#include "program.h"

struct rhi_RenderStats {
  int Vertices;
  int DrawCalls;
  int Triangles;
};

void rhi_RenderDevice_NewFrame();
void rhi_RenderDevice_UseProgram(struct rhi_Program);
void rhi_RenderDevice_Draw(struct rhi_VAO, int count);
void rhi_RenderDevice_DrawInstanced(struct rhi_VAO, int count, int instances);
struct rhi_RenderStats rhi_GetStats();