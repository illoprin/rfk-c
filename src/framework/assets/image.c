#include "image.h"

#include <stb/stb_image.h>

int img_from_file(Image2D* img, const char* path) {

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

Vector img_load_batch(Vector files, ImageCheckFunc check) {
  Vector images;
  Vec_Init(&images, Image2D);

  for (uint i = 0; i < files.Len; ++i) {
    // get path
    const char* path = Vec_GetRaw(&files, i);
    if (path == NULL) continue;

    // load image
    Image2D img;
    if (img_from_file(&img, path)) continue;

    // validate image
    if (check) {
      if (!check(img)) {
        img_destroy(img);
        continue;
      }
    }

    // push to array
    Vec_Push(&images, img);
  }

  return images;
}

void img_batch_destroy(Vector images) {
  for (size_t i = 0; i < images.Len; ++i) {
    Image2D* img = Vec_GetRaw(&images, i);
    if (!img) continue;
    img_destroy(*img);
  }
}

void img_destroy(Image2D img) {
  if (img.Pix) {
    free(img.Pix);
  }
}

void img_ptr_destroy(Image2D* img) {
  if (!img) return;
  if (img->Pix) {
    free(img->Pix);
  }
  free(img);
}