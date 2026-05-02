#pragma once

#include <kernel/gfx/rhi/program.h>

// Quick load vs and fs
int rhi_prog_quick_load(rhi_Program* prog, const char* vs_path, const char* fs_path);