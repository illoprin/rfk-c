#pragma once

#include <glad/glad.h>
#include <kernel/core/types.h>
#include <stdbool.h>

// Shader types
typedef enum {
  RHI_SHADER_VERTEX = GL_VERTEX_SHADER,
  RHI_SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
  RHI_SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
  RHI_SHADER_COMPUTE = GL_COMPUTE_SHADER
} rhi_ShaderType;

struct rhi_Program {
  uint32_t handle;
  bool is_linked;
};

// Basic API
void rhi_Prog_Init(struct rhi_Program* prog);
bool rhi_Prog_AddShader(struct rhi_Program* prog, rhi_ShaderType type, const char* source);
bool rhi_Prog_Link(struct rhi_Program* prog);
void rhi_Prog_Use(struct rhi_Program prog);
void rhi_Prog_Destroy(struct rhi_Program* prog);

// Uniform Setters
void rhi_Prog_SetInt(struct rhi_Program prog, const char* name, int value);
void rhi_Prog_SetUInt(struct rhi_Program prog, const char* name, uint value);
void rhi_Prog_SetVec2f(struct rhi_Program prog, const char* name, float*);
void rhi_Prog_SetVec3f(struct rhi_Program prog, const char* name, float*);
void rhi_Prog_SetVec4f(struct rhi_Program prog, const char* name, float*);
void rhi_Prog_SetVec2i(struct rhi_Program prog, const char* name, int*);
void rhi_Prog_SetMat4f(struct rhi_Program prog, const char* name, float*); // data - float[16]
void rhi_Prog_SetMat2f(struct rhi_Program prog, const char* name, float*); // data - float[4]
