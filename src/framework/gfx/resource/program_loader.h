#pragma once

#include <kernel/gfx/rhi/program.h>

// Quick load vs and fs
int Prog_QuickLoad(struct rhi_Program* prog, const char* vs_path, const char* fs_path);