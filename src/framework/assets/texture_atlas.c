#include "texture_atlas.h"

#include <kernel/core/log.h>
#include <stb/stb_image_write.h>
#include <string.h>

typedef enum : uch {
  ATLAS_IMG_LOAD_FAIL,
  ATLAS_IMG_NOT_SQUARE,
  ATLAS_IMG_BASE_MISMATCH,
  ATLAS_IMG_INVALID_COMPS,
} TexAtlas_imgLoadError;

const uch comp = 4;

TexAtlas_imgLoadError TexAtlas_loadImage(
  struct TextureAtlas* handle, size_t index, Vector* images, const char* fileName
) {
  struct Image2D img = { 0 };

  char filePath[_MAX_PATH] = FLS_TEXTURE_PATH("");
  strcat(filePath, fileName);

  if (Img_FromFile(&img, filePath)) {
    // TODO load null
    return ATLAS_IMG_LOAD_FAIL;
  }

  if (img.Width != img.Height) {
    Img_Destroy(img);
    return ATLAS_IMG_NOT_SQUARE;
  }

  if (img.Channels != comp) {
    Img_Destroy(img);
    return ATLAS_IMG_INVALID_COMPS;
  }

  if (handle->UnitSize == 0) {
    handle->UnitSize = img.Width;
  }
  else {
    if (img.Width != handle->UnitSize || img.Height != handle->UnitSize) {
      Img_Destroy(img);
      return ATLAS_IMG_BASE_MISMATCH;
    }
  }

  Vec_Push(images, img);

  return 0;
};

static void TexAtlas_freeImagesVector(Vector* images) {
  for (size_t i = 0; i < images->Len; ++i) {
    struct Image2D* img = (struct Image2D*)Vec_AtRaw(images, i);
    if (!img) continue;
    Img_Destroy(*img);
  }
  Vec_Destroy(images);
}

static void TexAtlas_findAtlasSize(struct TextureAtlas* handle, size_t len) {
  // find size of array
  uint arrayWidth = 2;
  uint arrayHeight = 2;
  bool horizontal = false;
  while (arrayWidth * arrayHeight < len) {
    if (horizontal)
      arrayWidth <<= 1;
    else
      arrayHeight <<= 1;

    horizontal = !horizontal;
  }

  // set params
  handle->Width = arrayWidth;
  handle->Height = arrayHeight;
}

static void TexAtlas_createArray(struct TextureAtlas* handle, Vector images) {
  uint totalPixelWidth = handle->Width * handle->UnitSize;
  uint totalPixelHeight = handle->Height * handle->UnitSize;

  size_t totalBytes = (size_t)totalPixelWidth * totalPixelHeight * comp;
  uch* pix = (uch*)malloc(totalBytes);
  RFK_ASSERT(pix != NULL, "failed to allocate atlas buffer");

  memset(pix, 0, totalBytes);

  for (uint i = 0; i < images.Len; ++i) {
    struct Image2D* img = Vec_AtRaw(&images, i);
    if (!img || !img->Pix) continue;

    uint gridX = i % handle->Width;
    uint gridY = i / handle->Width;

    uint startOffsetX = gridX * handle->UnitSize;
    uint startOffsetY = gridY * handle->UnitSize;

    for (uint row = 0; row < handle->UnitSize; ++row) {
      uch* srcRow = &img->Pix[row * handle->UnitSize * comp];

      size_t atlasRowIndex = (size_t)(startOffsetY + row) * totalPixelWidth;
      uch* dstRow = &pix[(atlasRowIndex + startOffsetX) * comp];

      memcpy(dstRow, srcRow, handle->UnitSize * comp);
    }
  }

  handle->Pix = pix;
}

int TexAtlas_InitFromFiles(struct TextureAtlas* handle, Vector names, const char* nullImage) {
  *(handle) = (struct TextureAtlas){ 0 };
  Vector images; // Vector<struct Image2D>
  Vec_Init(&images, struct Image2D);

  // load images to vector
  for (size_t i = 0; i < names.Len; ++i) {
    const char* file = Vec_AtRaw(&names, i);

    if (file == NULL) {
      LogWarn("empty file string at index %d", i);
      continue;
    };

    TexAtlas_imgLoadError res = TexAtlas_loadImage(handle, i, &images, file);
    if (!res) {
      continue;
    }

    switch (res) {
      case ATLAS_IMG_LOAD_FAIL:
        LogWarn("failed to load image [Path = %s, Index = %d]", file, i);
        break;
      case ATLAS_IMG_NOT_SQUARE:
        LogWarn("non square images not allowed");
        break;
      case ATLAS_IMG_BASE_MISMATCH:
        LogWarn("size of image [Name = \"%s\", Index = %d] does not match base size",
          file, i);
        break;
      default:
        LogWarn("image format not allowed [Name = \"%s\", Index = %d], png RGBA8 only", file, i);
        break;
    }
  };

  // check array size
  if (images.Len == 0) {
    Vec_Destroy(&images);
    LogInfo("empty set");
    return 1;
  }

  // find atlas size
  TexAtlas_findAtlasSize(handle, images.Len);

  // create array
  TexAtlas_createArray(handle, images);

  // free images
  TexAtlas_freeImagesVector(&images);

  return 0;
}

int TexAtlas_WriteToFile(const struct TextureAtlas* handle, const char* path) {
  size_t W = handle->Width * handle->UnitSize;
  if (!stbi_write_png(path, W, handle->Height * handle->UnitSize, 4, handle->Pix, W * comp)) {
    LogErr("failed to write atlas to image");
    return 1;
  };
  return 0;
}

void TexAtlas_Free(struct TextureAtlas handle) {
  if (handle.Pix) {
    free(handle.Pix);
  }
}
