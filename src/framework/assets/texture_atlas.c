#include "texture_atlas.h"

#include <kernel/core/log.h>
#include <stb/stb_image_write.h>
#include <string.h>

// internal error codes for image validation during atlas packing
typedef enum : uch {
  ATLAS_IMG_NOT_SQUARE = 1,
  ATLAS_IMG_BASE_MISMATCH,
  ATLAS_IMG_INVALID_COMPS,
} TexAtlas_ValidationResult;

// fixed component count (rgba)
const uch comp = 4u;

TexAtlas_ValidationResult TexAtlas_validateImage(
  struct TextureAtlas* handle, struct Image2D* img
) {

  if (img->Width != img->Height) {
    return ATLAS_IMG_NOT_SQUARE;
  }

  if (img->Channels != comp) {
    return ATLAS_IMG_INVALID_COMPS;
  }

  if (handle->UnitSize == 0) {
    handle->UnitSize = img->Width;
  }
  else {
    if (img->Width != handle->UnitSize || img->Height != handle->UnitSize) {
      return ATLAS_IMG_BASE_MISMATCH;
    }
  }

  return 0;
}

static void TexAtlas_findAtlasSize(struct TextureAtlas* handle, size_t len) {
  uint arrayWidth = 2;
  uint arrayHeight = 2;
  bool horizontal = true;

  // expand grid dimensions until all images fit
  while (arrayWidth * arrayHeight < len) {
    if (horizontal)
      arrayWidth <<= 1;
    else
      arrayHeight <<= 1;

    horizontal = !horizontal;
  }

  handle->Width = arrayWidth;
  handle->Height = arrayHeight;
}

static void TexAtlas_createArray(struct TextureAtlas* handle, Vector images) {
  uint totalPixelWidth = handle->Width * handle->UnitSize;
  uint totalPixelHeight = handle->Height * handle->UnitSize;

  // allocate memory for the entire atlas
  size_t totalBytes = (size_t)totalPixelWidth * totalPixelHeight * comp;
  uch* pix = (uch*)malloc(totalBytes);
  RFK_ASSERT(pix != NULL, "failed to allocate atlas buffer");

  // initialize with empty pixels
  memset(pix, 0, totalBytes);

  for (uint i = 0; i < images.Len; ++i) {
    struct Image2D* img = Vec_AtRaw(&images, i);
    if (!img || !img->Pix) continue;

    // determine unit coordinates in the grid
    uint gridX = i % handle->Width;
    uint gridY = i / handle->Width;

    // determine pixel offsets in the atlas
    uint startOffsetX = gridX * handle->UnitSize;
    uint startOffsetY = gridY * handle->UnitSize;

    // copy image rows into atlas buffer
    for (uint row = 0; row < handle->UnitSize; ++row) {
      uch* srcRow = &img->Pix[row * handle->UnitSize * comp];
      size_t atlasRowIndex = (size_t)(startOffsetY + row) * totalPixelWidth;
      uch* dstRow = &pix[(atlasRowIndex + startOffsetX) * comp];

      memcpy(dstRow, srcRow, handle->UnitSize * comp);
    }
  }

  handle->Pix = pix;
}

int TexAtlas_InitFromImages(struct TextureAtlas* handle, Vector images) {
  *(handle) = (struct TextureAtlas){ 0 };
  Vector validated;
  Vec_Init(&validated, struct Image2D);

  // validation
  for (size_t i = 0; i < images.Len; ++i) {
    struct Image2D* img = Vec_AtRaw(&images, i);
    if (!img) continue;

    // check image
    TexAtlas_ValidationResult r = TexAtlas_validateImage(handle, img);

    if (!r) {
      Vec_PushRaw(&validated, img);
      continue;
    }

    // log specific errors for failed image
    switch (r) {
      case ATLAS_IMG_NOT_SQUARE:
        LogWarn("non square images not allowed [index = %d]", i);
        break;
      case ATLAS_IMG_BASE_MISMATCH:
        LogWarn("size mismatch for image [index = %d]", i);
        break;
      default:
        LogWarn("unsupported format [index = %d]", i);
        break;
    }
  }

  if (validated.Len == 0) {
    LogErr("empty set");
    Vec_Destroy(&validated);
    return 1;
  }

  TexAtlas_findAtlasSize(handle, validated.Len);
  TexAtlas_createArray(handle, validated);
  Vec_Destroy(&validated);

  LogInfo("atlas created { W: %d H: %d }",
    handle->Width * handle->UnitSize,
    handle->Height * handle->UnitSize
  );
  return 0;
}

int TexAtlas_WriteToFile(const struct TextureAtlas* handle, const char* path) {
  size_t W = (size_t)handle->Width * handle->UnitSize;
  size_t H = (size_t)handle->Height * handle->UnitSize;

  // export buffer to png via stb_image_write
  if (!stbi_write_png(path, (int)W, (int)H, 4, handle->Pix, (int)(W * comp))) {
    LogErr("failed to write atlas to file: \"%s\"", path);
    return 1;
  };
  LogInfo("atlas written to file: \"%s\"", path);

  return 0;
}

void TexAtlas_Free(struct TextureAtlas* handle) {
  if (handle->Pix) {
    free(handle->Pix);
  }
}