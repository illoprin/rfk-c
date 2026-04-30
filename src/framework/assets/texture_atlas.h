#pragma once

#include <kernel/core/types.h>
#include <vector/vector.h>
#include <framework/assets/image.h>
#include <kernel/core/files.h>

/// @brief represents a 2d grid of square texture units combined into a single buffer
struct TextureAtlas {
  uch* Pix;      // raw pixel data buffer
  uint Width;    // atlas width in units
  uint Height;   // atlas height in units
  uint UnitSize; // size of a single square unit in pixels
};

/// @brief initializes atlas by loading and packing multiple images
/// @param handle pointer to the atlas structure to initialize
/// @param images vector containing loaded images
/// @return 0 on success, 1 if no images were loaded
int TexAtlas_InitFromImages(struct TextureAtlas* handle, Vector images);

/// @brief saves the generated texture atlas to a png file
/// @param handle pointer to the atlas structure
/// @param path destination file path
/// @returns 0 on success, 1 on write failure
int TexAtlas_WriteToFile(const struct TextureAtlas* handle, const char* path);

/// @brief deallocates the pixel buffer of the texture atlas
/// @param handle atlas structure containing the buffer to free
void TexAtlas_Free(struct TextureAtlas* handle);