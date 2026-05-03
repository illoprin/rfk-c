#include "program_loader.h"

#include <kernel/core/files.h>
#include <stdlib.h>

bool rhi_prog_quick_load(
  rhi_Program* prog, const char* vs_path, const char* fs_path
) {
  // init program
  rhi_prog_init(prog);

  // read shader files
  char* vs_src = fls_read_file(vs_path, NULL);
  char* fs_src = fls_read_file(fs_path, NULL);
  if (!vs_src || !fs_src) {
    fprintf(stderr, "failed to load shader\n");
    return false;
  }

  // add shader files and link program
  int res = rhi_prog_add_shader(prog, RHI_SHADER_VERTEX, vs_src)
            && rhi_prog_add_shader(prog, RHI_SHADER_FRAGMENT, fs_src)
            && rhi_prog_link(prog);

  // free resources
  free(vs_src);
  free(fs_src);

  // return
  return res;
}
