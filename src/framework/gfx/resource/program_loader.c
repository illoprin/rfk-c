#include "program_loader.h"

#include <kernel/core/files.h>
#include <stdlib.h>

int Prog_QuickLoad(rhi_Program* prog, const char* vs_path, const char* fs_path) {
  rhi_prog_init(prog);

  size_t vs_size, fs_size;
  char* vs_src = fls_read_file(vs_path, &vs_size);
  char* fs_src = fls_read_file(fs_path, &fs_size);

  if (!vs_src || !fs_src) {
    fprintf(stderr, "failed to load shader\n");
    return 1;
  }

  bool res = rhi_prog_add_shader(prog, RHI_SHADER_VERTEX, vs_src) &&
    rhi_prog_add_shader(prog, RHI_SHADER_FRAGMENT, fs_src) &&
    rhi_prog_link(prog);

  free(vs_src);
  free(fs_src);
  return res;
}
