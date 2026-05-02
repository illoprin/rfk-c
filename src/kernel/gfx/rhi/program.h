#pragma once

#include "util.h"

// Program object handle
typedef struct {
  uint32_t handle;
  bool is_linked;
} rhi_Program;

// Basic API
void rhi_prog_init(rhi_Program* prog);
bool rhi_prog_add_shader(rhi_Program* prog, rhi_ShaderType type, const char* source);
bool rhi_prog_link(rhi_Program* prog);
void rhi_prog_invalidate(rhi_Program* prog);

// Uniform Setters
void rhi_prog_uniform_1i(rhi_Program prog, const char* name, int value);
void rhi_prog_uniform_1ui(rhi_Program prog, const char* name, int value);
void rhi_prog_uniform_1f(rhi_Program prog, const char* name, float);
void rhi_prog_uniform_2f(rhi_Program prog, const char* name, float*);
void rhi_prog_uniform_3f(rhi_Program prog, const char* name, float*);
void rhi_prog_uniform_4f(rhi_Program prog, const char* name, float*);
void rhi_prog_uniform_2i(rhi_Program prog, const char* name, int*);
void rhi_prog_uniform_mat4(rhi_Program prog, const char* name, float*); // data - float[16]
void rhi_prog_uniform_mat2(rhi_Program prog, const char* name, float*); // data - float[4]