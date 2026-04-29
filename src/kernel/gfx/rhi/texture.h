#pragma once

#include "util.h"

struct rhi_TextureConfig {
  rhi_TextureFormat Format;
  rhi_TextureFilter FilterMin;
  rhi_TextureFilter FilterMag;
  rhi_TextureWrap Wrap;
  uint Width, Height;
};

struct rhi_Texture {
  GLuint ID;
  rhi_TextureType type;
  uint width, height, depth;
};

/// @brief binds texture into sampler unit
/// @param tex target texture object
/// @param unit index of sampler unit
void rhi_Tex_BindToUnit(struct rhi_Texture tex, uint unit);

/// @brief create texture object
/// @param type sampler type (2d, array, cube)
void rhi_Tex_Init(struct rhi_Texture* t, rhi_TextureType type);

void rhi_Tex_GenMipmaps(struct rhi_Texture t);

/// @brief deletes texture object
void rhi_Tex_Invalidate(struct rhi_Texture* t);

/// @brief allocates memory for texture 2D storage
/// @param t handle
/// @param conf sampler config
/// @param pixFormat source data format (e.g. RHI_RGBA)
/// @param sourceType source data type (e. g. RHI_UNSIGNED_BYTE)
/// @param data source data (if null - only allocate)
void rhi_Tex2D_Allocate(
  struct rhi_Texture* t,
  struct rhi_TextureConfig conf,
  uint pixFormat,
  uint sourceType,
  void* data
);

/// @brief updates data in texture 2d storage
/// @param t handle
/// @param x offset x
/// @param y offset y
/// @param width update region width
/// @param height update region height
/// @param pixFormat source data format (e.g. RHI_RGBA)
/// @param sourceType source data type (e. g. RHI_UNSIGNED_BYTE)
/// @param data source data (cannot be null)
void rhi_Tex2D_Update(
  struct rhi_Texture* t,
  uint x,
  uint y,
  uint width,
  uint height,
  uint pixFormat,
  uint sourceType,
  void* data
);