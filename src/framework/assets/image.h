#pragma once

#include <rfklib/log.h>
#include <rfklib/vector.h>

typedef struct {
  uch* Pix;
  uint Channels;
  uint Width;
  uint Height;
} Image2D;

typedef bool (*ImageCheckFunc)(Image2D);

/// @brief reads image into structure (allocates memory)
int img_from_file(Image2D*, const char* path);

/// @brief create vector of images from files
/// @param files vector of file pathes
/// @param func loaded image validation func
/// @return Vector<Image2D>
Vector img_load_batch(Vector files, ImageCheckFunc func);

/// @brief free pixels of images
/// !!! the vector destroying separately
/// call Vec_Destroy() to invalidate meta data
/// @param images Vector<Image2D>
void img_batch_destroy(Vector images);

/// @brief free Pix array
void img_destroy(Image2D);

/// @brief free whole structure (if allocated on heap)
void img_ptr_destroy(Image2D*);
