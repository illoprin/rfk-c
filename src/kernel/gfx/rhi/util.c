#include "util.h"

uint rhi_util_get_data_type(rhi_TextureFormat fmt) {
  // byte
  if (fmt == RHI_TEX_FORMAT_R8
    || fmt == RHI_TEX_FORMAT_RGB8
    || fmt == RHI_TEX_FORMAT_RGBA8) {
    return RHI_UNSIGNED_BYTE;
  }

  // half-float
  if (fmt == RHI_TEX_FORMAT_R16F
    || fmt == RHI_TEX_FORMAT_RGB16F
    || fmt == RHI_TEX_FORMAT_RGBA16F) {
    return GL_HALF_FLOAT;
  }

  // float
  if (fmt == RHI_TEX_FORMAT_R32F
    || fmt == RHI_TEX_FORMAT_RGB32F
    || fmt == RHI_TEX_FORMAT_RGBA32F
    || fmt == RHI_TEX_FORMAT_DEPTH32F) {
    return RHI_FLOAT;
  }

  // unsigned short
  if (fmt == RHI_TEX_FORMAT_DEPTH16) {
    return GL_UNSIGNED_SHORT;
  }

  // unsigned int
  if (fmt == RHI_TEX_FORMAT_DEPTH24
    || fmt == RHI_TEX_FORMAT_DEPTH32) {
    return RHI_UNSIGNED_INT;
  }

  // unsigned int_24_8
  if (fmt == RHI_TEX_FORMAT_DEPTH24_STENCIL8) {
    return GL_UNSIGNED_INT_24_8;
  }

  return GL_UNSIGNED_BYTE;
}

bool rhi_util_is_depth_fmt(rhi_TextureFormat fmt) {
  // depth component
  if (fmt == RHI_TEX_FORMAT_DEPTH16
    || fmt == RHI_TEX_FORMAT_DEPTH24
    || fmt == RHI_TEX_FORMAT_DEPTH32
    || fmt == RHI_TEX_FORMAT_DEPTH32F
    || fmt == RHI_TEX_FORMAT_DEPTH24_STENCIL8) {
    return true;
  }
  return false;
}


uint rhi_util_get_pix_fmt(rhi_TextureFormat fmt) {
  // rgba
  if (fmt == RHI_TEX_FORMAT_RGBA16F
    || fmt == RHI_TEX_FORMAT_RGBA32F
    || fmt == RHI_TEX_FORMAT_RGBA8) {
    return GL_RGBA;
  }

  // rgb
  if (fmt == RHI_TEX_FORMAT_RGB16F
    || fmt == RHI_TEX_FORMAT_RGB32F
    || fmt == RHI_TEX_FORMAT_RGB8) {
    return GL_RGB;
  }

  // red
  if (fmt == RHI_TEX_FORMAT_R16F
    || fmt == RHI_TEX_FORMAT_R32F
    || fmt == RHI_TEX_FORMAT_R8) {
    return GL_RED;
  }

  // depth component
  if (fmt == RHI_TEX_FORMAT_DEPTH16
    || fmt == RHI_TEX_FORMAT_DEPTH24
    || fmt == RHI_TEX_FORMAT_DEPTH32
    || fmt == RHI_TEX_FORMAT_DEPTH32F) {
    return GL_DEPTH_COMPONENT;
  }

  // depth stencil
  if (fmt == RHI_TEX_FORMAT_DEPTH24_STENCIL8) {
    return GL_DEPTH_STENCIL;
  }

  return GL_RGBA;
}