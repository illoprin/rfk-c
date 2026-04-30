#pragma once

#include <kernel/core/types.h>

struct Image2D {
  uch* Pix;
  int Channels;
  int Width;
  int Height;
};

/// @brief reads image into structure (allocates memory)
int Img_FromFile(struct Image2D*, const char* path);

void Img_Free(struct Image2D);
void ImgPtr_Free(struct Image2D*);
