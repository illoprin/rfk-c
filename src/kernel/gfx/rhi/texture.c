#include "texture.h"

static void rhi_setTexParameters(
  uint32_t handle,
  rhi_TextureFilter filterMin,
  rhi_TextureFilter filterMag,
  rhi_TextureWrap wrap
) {
  glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, filterMin);
  glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, filterMag);
  glTextureParameteri(handle, GL_TEXTURE_WRAP_S, wrap);
  glTextureParameteri(handle, GL_TEXTURE_WRAP_T, wrap);
  glTextureParameteri(handle, GL_TEXTURE_WRAP_R, wrap);
}

struct rhi_Texture rhi_Tex2D_Create(
  int w, int h,
  struct rhi_TextureConfig conf
) {
  struct rhi_Texture tex;
  tex.type = RHI_TEXTURE_2D;
  glCreateTextures(GL_TEXTURE_2D, 1, &tex.handle);
  glTextureStorage2D(tex.handle, 1, conf.Format, w, h);
  rhi_setTexParameters(tex.handle, conf.FilterMin, conf.FilterMag, conf.Wrap);
  return tex;
}

void rhi_Tex2D_Update(struct rhi_Texture tex, int x, int y, int w, int h, GLenum pixel_format, GLenum pixel_type, const void* data) {
  if (tex.type != RHI_TEXTURE_2D) {
    LogErr("rhi: type mismatch in Tex2D_Update\n");
    return;
  }
  glTextureSubImage2D(tex.handle, 0, x, y, w, h, pixel_format, pixel_type, data);
}

struct rhi_Texture rhi_Tex2DArray_Create(int w, int h, int layers, struct rhi_TextureConfig conf) {
  struct rhi_Texture tex;
  tex.type = RHI_TEXTURE_2D_ARRAY;
  glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tex.handle);

  glTextureStorage3D(tex.handle, 1, conf.Format, w, h, layers);

  rhi_setTexParameters(tex.handle, conf.FilterMin, conf.FilterMag, conf.Wrap);
  return tex;
}

void rhi_Tex2DArray_Update(
  struct rhi_Texture tex,
  int x, int y, int layer,
  int w, int h,
  GLenum p_fmt, GLenum p_type,
  const void* data
) {
  if (tex.type != RHI_TEXTURE_2D_ARRAY) return;
  glTextureSubImage3D(tex.handle, 0, x, y, layer, w, h, 0, p_fmt, p_type, data);
}

struct rhi_Texture rhi_TexCube_Create(int size, struct rhi_TextureConfig conf) {
  struct rhi_Texture tex;
  tex.type = RHI_TEXTURE_CUBE_MAP;
  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex.handle);
  glTextureStorage2D(tex.handle, 1, conf.Format, size, size);
  rhi_setTexParameters(tex.handle, conf.FilterMin, conf.FilterMag, conf.Wrap);
  return tex;
}

void rhi_TexCube_Update(
  struct rhi_Texture tex, int x, int y, int face,
  int w, int h, int count,
  uint32_t p_fmt, uint32_t p_type,
  const void* data
) {
  if (tex.type != RHI_TEXTURE_CUBE_MAP) return;
  // face - это zoffset, count - это depth
  glTextureSubImage3D(tex.handle, 0, x, y, face, w, h, count, p_fmt, p_type, data);
}

void rhi_Tex_Destroy(struct rhi_Texture tex) {
  glDeleteTextures(1, &tex.handle);
}
