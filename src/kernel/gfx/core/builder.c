#include "builder.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <kernel/core/files.h>
#include <vector/vector.h>

int Prog_QuickLoad(struct rhi_Program* prog, const char* vs_path, const char* fs_path) {
  rhi_Prog_Init(prog);

  size_t vs_size, fs_size;
  char* vs_src = Fls_ReadFile(vs_path, &vs_size);
  char* fs_src = Fls_ReadFile(fs_path, &fs_size);

  if (!vs_src || !fs_src) {
    fprintf(stderr, "failed to load shader\n");
    return 1;
  }

  bool res = rhi_Prog_AddShader(prog, RHI_SHADER_VERTEX, vs_src) &&
    rhi_Prog_AddShader(prog, RHI_SHADER_FRAGMENT, fs_src) &&
    rhi_Prog_Link(prog);

  free(vs_src);
  free(fs_src);
  return res;
}
