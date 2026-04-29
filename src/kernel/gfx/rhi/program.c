#include "program.h"

#include <kernel/core/files.h>
#include <kernel/core/log.h>
#include <stdio.h>
#include <stdlib.h>

void rhi_Prog_Init(struct rhi_Program* prog) {
  prog->handle = glCreateProgram();
  prog->is_linked = false;
  LogInfo("program [ID = %d] created", prog->handle);
}

bool rhi_Prog_AddShader(struct rhi_Program* prog, rhi_ShaderType type, const char* source) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    LogErr("shader compilation error - \n%s\n", infoLog);
    glDeleteShader(shader);
    return false;
  }

  glAttachShader(prog->handle, shader);
  glDeleteShader(shader);
  return true;
}

bool rhi_Prog_Link(struct rhi_Program* prog) {
  glLinkProgram(prog->handle);

  int success;
  glGetProgramiv(prog->handle, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(prog->handle, 512, NULL, infoLog);
    LogErr("program linking error - \n%s\n", infoLog);
    return false;
  }

  prog->is_linked = true;
  return true;
}

void rhi_Prog_Use(struct rhi_Program prog) {
  glUseProgram(prog.handle);
}

// TODO cache uniform locations

void rhi_Prog_SetInt(struct rhi_Program prog, const char* name, int value) {
  glProgramUniform1i(prog.handle, glGetUniformLocation(prog.handle, name), value);
}

void rhi_Prog_SetUInt(struct rhi_Program prog, const char* name, uint value) {
  glProgramUniform1ui(prog.handle, glGetUniformLocation(prog.handle, name), value);
}

void rhi_Prog_SetVec2f(struct rhi_Program prog, const char* name, float* v) {
  glProgramUniform2fv(prog.handle, glGetUniformLocation(prog.handle, name), 1, v);
}

void rhi_Prog_SetVec3f(struct rhi_Program prog, const char* name, float* v) {
  glProgramUniform3fv(prog.handle, glGetUniformLocation(prog.handle, name), 1, v);
}

void rhi_Prog_SetVec4f(struct rhi_Program prog, const char* name, float* v) {
  glProgramUniform4fv(prog.handle, glGetUniformLocation(prog.handle, name), 1, v);
}

void rhi_Prog_SetVec2i(struct rhi_Program prog, const char* name, int* v) {
  glProgramUniform2iv(prog.handle, glGetUniformLocation(prog.handle, name), 1, v);
}

void rhi_Prog_SetMat4f(struct rhi_Program prog, const char* name, float* data) {
  glProgramUniformMatrix4fv(prog.handle, glGetUniformLocation(prog.handle, name), 1, GL_FALSE, data);
}

void rhi_Prog_SetMat2f(struct rhi_Program prog, const char* name, float* data) {
  glProgramUniformMatrix2fv(prog.handle, glGetUniformLocation(prog.handle, name), 1, GL_FALSE, data);
}

void rhi_Prog_Destroy(struct rhi_Program* prog) {
  glDeleteProgram(prog->handle);
  LogInfo("program [ID = %d] deleted", prog->handle);
  prog->handle = 0;
}