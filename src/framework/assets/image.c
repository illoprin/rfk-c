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

void Img_Destroy(struct Image2D img) {
  if (img.Pix) {
    free(img.Pix);
  }
}

void ImgPtr_Free(struct Image2D* img) {
  if (!img) return;
  if (img->Pix) {
    free(img->Pix);
  }
  free(img);
}