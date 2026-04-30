#pragma once

#include <kernel/core/types.h>
#include <vector/vector.h>

struct Image2D {
  uch* Pix;
  uint Channels;
  uint Width;
  uint Height;
};

typedef bool (*ImageCheckFunc)(struct Image2D);

/// @brief reads image into structure (allocates memory)
int Img_FromFile(struct Image2D*, const char* path);

/// @brief create vector of images from files
/// @param files vector of file pathes
/// @param func loaded image validation func
/// @return Vector<struct Image2D>
Vector Img_LoadImagesFromFiles(Vector files, ImageCheckFunc func);

/// @brief free pixels of images
/// !!! the vector destroying separately
/// call Vec_Destroy() to invalidate meta data
/// @param images Vector<struct Image2D>
void Img_DestroyVector(Vector images);

/// @brief free pix array
void Img_Destroy(struct Image2D);

/// @brief free whole structure (if allocated on heap)
void ImgPtr_Destroy(struct Image2D*);
