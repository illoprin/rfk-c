#include "image.h"

#include <stb/stb_image.h>

#include <kernel/core/log.h>

int Img_FromFile(struct Image2D* img, const char* path) {

  int w, h, c;

  stbi_set_flip_vertically_on_load(1);

  uch* data = stbi_load(path, &w, &h, &c, 0);
  if (!data) {
    LogErr("failed to load image \"%s\"", path);
    return 1;
  }

  img->Pix = data;
  img->Width = (uint)w;
  img->Height = (uint)h;
  img->Channels = (uint)c;

  LogInfo("image \"%s\" loaded (%dx%d, %d channels)", path, w, h, c);

  return 0;
}

Vector Img_LoadImagesFromFiles(Vector files, ImageCheckFunc check) {
  Vector images;
  Vec_Init(&images, struct Image2D);

  for (uint i = 0; i < files.Len; ++i) {
    // get path
    const char* path = Vec_AtRaw(&files, i);
    if (path == NULL) continue;

    // load image
    struct Image2D img;
    if (Img_FromFile(&img, path)) continue;

    // validate image
    if (check) {
      if (!check(img)) {
        Img_Destroy(img);
        continue;
      }
    }

    // push to array
    Vec_Push(&images, img);
  }

  return images;
}

void Img_DestroyVector(Vector images) {
  for (size_t i = 0; i < images.Len; ++i) {
    struct Image2D* img = Vec_AtRaw(&images, i);
    if (!img) continue;
    Img_Destroy(*img);
  }
}

void Img_Destroy(struct Image2D img) {
  if (img.Pix) {
    free(img.Pix);
  }
}

void ImgPtr_Destroy(struct Image2D* img) {
  if (!img) return;
  if (img->Pix) {
    free(img->Pix);
  }
  free(img);
}