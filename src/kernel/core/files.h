#pragma once

#include <rfklib/types.h>
#include <stddef.h>

#define FLS_ASSETS_DIR   "assets"
#define FLS_TEXTURES_DIR "textures"
#define FLS_MODELS_DIR   "models"
#define FLS_SHADERS_DIR  "shaders"
#define FLS_FONTS_DIR    "fonts"

#define FLS_TEXTURE_PATH(f) FLS_ASSETS_DIR "/" FLS_TEXTURES_DIR "/" f

#define FLS_SHADER_PATH(f) FLS_ASSETS_DIR "/" FLS_SHADERS_DIR "/" f

#define FLS_MODEL_PATH(f) FLS_ASSETS_DIR "/" FLS_MODELS_DIR "/" f

#define FLS_FONT_PATH(f) FLS_ASSETS_DIR "/" FLS_FONTS_DIR "/" f

/// @brief reads file to string (allocates memory)
/// @param fpath path to file
/// @param out_size allocated buffer size
/// @return buffer with bytes
char* fls_read_file(const char* fpath, size_t* out_size);
