#include "program.h"

#include <stdlib.h>

uint progBinding = 0;

void rhi_Prog_Init(struct rhi_Program* prog) {
  if (prog == NULL) return;
  *prog = (struct rhi_Program){ 0 };

  prog->handle = glCreateProgram();
  prog->is_linked = false;
  LogInfo("program [ID = %d] created", prog->handle);
}

int rhi_Prog_getUniformLoc(struct rhi_Program prog, const char* name) {
  int loc = glGetUniformLocation(prog.handle, name);
  if (loc == -1) {
    LogWarn("program [ID = %d] uniform \"%s\" not found", prog.handle, name);
  }
  return loc;
}

bool rhi_Prog_AddShader(struct rhi_Program* prog, rhi_ShaderType type, const char* source) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success, logLen;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    char* logStr = (char*)malloc(logLen);
    glGetShaderInfoLog(shader, logLen, NULL, logStr);
    LogErr("shader compilation error - \n%s\n", logStr);
    glDeleteShader(shader);
    free(logStr);
    return false;
  }

  glAttachShader(prog->handle, shader);
  glDeleteShader(shader);
  return true;
}

bool rhi_Prog_Link(struct rhi_Program* prog) {
  if (prog == NULL) return;
  if (prog->handle == 0) return;
  glLinkProgram(prog->handle);

  int success;
  uint logLen;
  glGetProgramiv(prog->handle, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramiv(prog->handle, GL_INFO_LOG_LENGTH, &logLen);
    char* logStr = (char*)malloc(logLen);
    glGetProgramInfoLog(prog->handle, logLen, NULL, logStr);
    LogErr("program linking error - \n%s\n", logStr);
    free(logStr);
    return false;
  }

  prog->is_linked = true;
  return true;
}

void rhi_Prog_Use(struct rhi_Program prog) {
  if (prog.handle == 0) return;
  if (progBinding == prog.handle) return;
  glUseProgram(prog.handle);
  progBinding = prog.handle;
}

// TODO cache uniform locations

void rhi_Prog_SetInt(struct rhi_Program prog, const char* name, int value) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform1i(loc, value);
}

void rhi_Prog_SetUInt(struct rhi_Program prog, const char* name, uint value) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform1ui(loc, value);
}

void rhi_Prog_SetVec2f(struct rhi_Program prog, const char* name, float* v) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform2fv(loc, 1, v);
}

void rhi_Prog_SetVec3f(struct rhi_Program prog, const char* name, float* v) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform3fv(loc, 1, v);
}

void rhi_Prog_SetVec4f(struct rhi_Program prog, const char* name, float* v) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform4fv(loc, 1, v);
}

void rhi_Prog_SetVec2i(struct rhi_Program prog, const char* name, int* v) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniform2iv(loc, 1, v);
}

void rhi_Prog_SetMat4f(struct rhi_Program prog, const char* name, float* data) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}

void rhi_Prog_SetMat2f(struct rhi_Program prog, const char* name, float* data) {
  rhi_Prog_Use(prog);
  int loc = rhi_Prog_getUniformLoc(prog, name);
  if (loc < 0) return;
  glUniformMatrix2fv(loc, 1, GL_FALSE, data);
}

void rhi_Prog_Invalidate(struct rhi_Program* prog) {
  glDeleteProgram(prog->handle);
  LogInfo("program [ID = %d] deleted", prog->handle);
  *prog = (struct rhi_Program){ 0 };
}