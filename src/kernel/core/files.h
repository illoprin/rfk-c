#pragma once

#include "types.h"
#include <stddef.h>

#define FLS_ASSETS_DIR "assets"
#define FLS_TEXTURES_DIR "textures"
#define FLS_MODELS_DIR "models"
#define FLS_SHADERS_DIR "shaders"

#define FLS_TEXTURE_PATH(f) \
  FLS_ASSETS_DIR "/" FLS_TEXTURES_DIR "/" f

#define FLS_SHADER_PATH(f) \
  FLS_ASSETS_DIR "/" FLS_SHADERS_DIR "/" f

#define FLS_MODEL_PATH(f) \
  FLS_ASSETS_DIR "/" FLS_MODELS_DIR "/" f

struct Image2D {
  uch* Pix;
  int Channels;
  int Width;
  int Height;
};

/// @brief reads file to string (allocates memory)
/// @param fpath path to file
/// @param out_size allocated buffer size
/// @return buffer with bytes
char* Fls_ReadFile(const char* fpath, size_t* out_size);


/// @brief reads image into structure (allocates memory)
struct Image2D Fls_ReadImage(const char* path);

void Img_Free(struct Image2D);
void ImgPtr_Free(struct Image2D*);

