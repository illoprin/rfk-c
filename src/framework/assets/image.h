#pragma once

#include <kernel/core/types.h>

struct Image2D {
  uch* Pix;
  uint Channels;
  uint Width;
  uint Height;
};

/// @brief reads image into structure (allocates memory)
int Img_FromFile(struct Image2D*, const char* path);

void Img_Destroy(struct Image2D);
void ImgPtr_Free(struct Image2D*);
