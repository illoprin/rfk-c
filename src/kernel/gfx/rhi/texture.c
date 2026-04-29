#include "texture.h"

static void rhi_Tex_setTexParameters(
  int target,
  rhi_TextureFilter filterMin,
  rhi_TextureFilter filterMag,
  rhi_TextureWrap wrap
) {
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterMin);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterMag);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);
}


void rhi_Tex_BindToUnit(struct rhi_Texture tex, int unit) {
  if (tex.ID == 0) return;
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(tex.type, tex.ID);
}

void rhi_Tex_Init(struct rhi_Texture* t, rhi_TextureType type) {
  if (t == NULL) return;
  *t = (struct rhi_Texture){ 0 };

  t->type = type;
  glGenTextures(1, &t->ID);
  glBindTexture(t->type, t->ID);
  LogInfo("texture [ID = %d, Type = 0x%08x] created ", t->ID, t->type);
}

void rhi_Tex_Invalidate(struct rhi_Texture* t) {
  if (t == NULL) return;
  if (t->ID == 0) return;
  glDeleteTextures(1, &t->ID);
  LogInfo("texture [ID = %d, Type = 0x%08x] deleted", t->ID, t->type);
  *t = (struct rhi_Texture){ 0 };
}

void rhi_Tex_GenMipmaps(struct rhi_Texture t) {
  if (t.ID == 0) return;

  glBindTexture(t.type, t.ID);
  glGenerateMipmap(t.type);
}

void rhi_Tex2D_Allocate(
  struct rhi_Texture* t,
  struct rhi_TextureConfig conf,
  void* pix
) {
  if (t == NULL) return;
  if (t->ID == 0) return;
  if (t->type != RHI_TEX_2D) return;

  t->width = conf.Width;
  t->height = conf.Height;
  t->fmt = conf.Format;
  t->sourceType = conf.SourceType;

  glBindTexture(GL_TEXTURE_2D, t->ID);
  rhi_setTexParameters(t->type, conf.FilterMin, conf.FilterMag, conf.Wrap);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(t->type, 0, t->fmt, conf.Width, conf.Height, 0, rhi_getPixelFormat(t->fmt), t->sourceType, pix);
}

void rhi_Tex2D_Resize(
  struct rhi_Texture* t,
  uint width, uint height
) {
  if (t == NULL) return;
  if (t->ID == 0) return;

  t->width = width;
  t->height = height;

  glBindTexture(GL_TEXTURE_2D, t->ID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(t->type, 0, t->fmt, t->width, t->height, 0, rhi_getPixelFormat(t->fmt), t->sourceType, NULL);
}

void rhi_Tex2D_Update(
  struct rhi_Texture* t,
  int x,
  int y,
  int width,
  int height,
  void* data
) {
  if (t == NULL) return;
  if (t->ID == 0) return;
  if (t->type != RHI_TEX_2D) return;
  if (x + width > t->width || y + height > t->height) return;
  glBindTexture(GL_TEXTURE_2D, t->ID);
  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, rhi_getPixelFormat(t->fmt), t->sourceType, data);
}

void rhi_Tex3D_Allocate(
  struct rhi_Texture* t,
  struct rhi_TextureConfig conf,
  uint depth,
  void* pix
) {
  if (t == NULL || t->ID == 0) return;
  if (t->type != GL_TEXTURE_3D && t->type != GL_TEXTURE_2D_ARRAY) return;

  t->width = conf.Width;
  t->height = conf.Height;
  t->depth = depth;

  t->fmt = conf.Format;
  t->sourceType = conf.SourceType;

  glBindTexture(t->type, t->ID);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  rhi_setTexParameters(t->type, conf.FilterMin, conf.FilterMag, conf.Wrap);

  glTexImage3D(
    t->type,
    0,
    t->fmt,
    conf.Width,
    conf.Height,
    depth,
    0,
    rhi_getPixelFormat(t->fmt),
    t->sourceType,
    pix
  );
}

void rhi_Tex3D_Update(
  struct rhi_Texture* t,
  uint x, uint y, uint z,
  uint width, uint height, uint depth,
  void* data
) {
  if (t == NULL || t->ID == 0 || data == NULL) return;

  if (x + width > t->width || y + height > t->height || z + depth > t->depth) {
    LogWarn("Texture3D Update out of bounds");
    return;
  }

  glBindTexture(t->type, t->ID);

  glTexSubImage3D(
    t->type,
    0,
    x, y, z,
    width, height, depth,
    rhi_getPixelFormat(t->fmt),
    t->sourceType,
    data
  );
}
