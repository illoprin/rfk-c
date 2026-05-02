#include "program.h"

#include <stdlib.h>

void rhi_prog_init(rhi_Program* prog) {
  if (prog == NULL) return;
  *prog = (rhi_Program){ 0 };

  prog->handle = glCreateProgram();
  prog->is_linked = false;
  LogInfo("program [ID = %d] created", prog->handle);
}

static int get_uniform_loc(rhi_Program prog, const char* name) {
  int loc = glGetUniformLocation(prog.handle, name);
  if (loc == -1) {
    LogWarn("program [ID = %d] uniform \"%s\" not found", prog.handle, name);
  }
  return loc;
}

bool rhi_prog_add_shader(rhi_Program* prog, rhi_ShaderType type, const char* source) {
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

bool rhi_prog_link(rhi_Program* prog) {
  if (prog == NULL) return false;
  if (prog->handle == 0) return false;
  glLinkProgram(prog->handle);

  int success, logLen;
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

// TODO cache uniform locations

void rhi_prog_uniform_1i(rhi_Program prog, const char* name, int value) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform1i(loc, value);
}

void rhi_prog_uniform_1ui(rhi_Program prog, const char* name, int value) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform1ui(loc, value);
}

void rhi_prog_uniform_1f(rhi_Program prog, const char* name, float v) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform1f(loc, v);
}

void rhi_prog_uniform_2f(rhi_Program prog, const char* name, float* v) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform2fv(loc, 1, v);
}

void rhi_prog_uniform_3f(rhi_Program prog, const char* name, float* v) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform3fv(loc, 1, v);
}

void rhi_prog_uniform_4f(rhi_Program prog, const char* name, float* v) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform4fv(loc, 1, v);
}

void rhi_prog_uniform_2i(rhi_Program prog, const char* name, int* v) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniform2iv(loc, 1, v);
}

void rhi_prog_uniform_mat4(rhi_Program prog, const char* name, float* data) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}

void rhi_prog_uniform_mat2(rhi_Program prog, const char* name, float* data) {
  int loc = get_uniform_loc(prog, name);
  if (loc < 0) return;
  glUniformMatrix2fv(loc, 1, GL_FALSE, data);
}

void rhi_prog_invalidate(rhi_Program* prog) {
  glDeleteProgram(prog->handle);
  LogInfo("program [ID = %d] deleted", prog->handle);
  *prog = (rhi_Program){ 0 };
}