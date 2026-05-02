#pragma once

#include "util.h"

typedef struct {
  rhi_TextureFormat Fmt;
  rhi_DataType SrcType;
  rhi_TextureFilter MinFilter;
  rhi_TextureFilter MagFilter;
  rhi_TextureWrap Wrap;
  int Width, Height;
} rhi_TextureConfig;

typedef struct {
  GLuint ID;
  rhi_TextureType type;
  rhi_TextureFormat fmt;
  rhi_DataType src_type;
  int width, height, depth;
} rhi_Texture;

/// @brief binds texture into sampler unit
/// @param tex target texture object
/// @param unit index of sampler unit
void rhi_tex_bind_to_unit(rhi_Texture tex, int unit);

/// @brief create texture object
/// @param type sampler type (2d, array, cube)
void rhi_tex_init(rhi_Texture* t, rhi_TextureType type);

void rhi_tex_gen_mips(rhi_Texture t);

/// @brief deletes texture object
void rhi_tex_invalidate(rhi_Texture* t);

/// @brief allocates memory for texture 2D storage
/// @param t handle
/// @param conf sampler config
/// @param pixFormat source data format (e.g. RHI_RGBA)
/// @param sourceType source data type (e. g. RHI_UNSIGNED_BYTE)
/// @param data source data (if null - only allocate)
void rhi_tex2d_alloc(
  rhi_Texture* t,
  rhi_TextureConfig conf,
  void* data
);

/// @brief updates data in texture 2d storage
/// @param t handle
/// @param x offset x
/// @param y offset y
/// @param width update region width
/// @param height update region height
/// @param sourceType source data type (e. g. RHI_UNSIGNED_BYTE)
/// @param data source data (cannot be null)
void rhi_tex2d_update(
  rhi_Texture* t,
  int x,
  int y,
  int width,
  int height,
  void* data
);

/// @brief allocates new memory for texture 2d
/// object and fills null
void rhi_tex2d_resize(rhi_Texture* t, uint width, uint height);

/// @brief allocates memory for 3D texture or 2D Array
void rhi_tex3d_alloc(
  rhi_Texture* t,
  rhi_TextureConfig conf,
  uint depth,
  void* pix
);

/// @brief updates data in 3D/Array texture storage
void rhi_tex3d_update(
  rhi_Texture* t,
  uint x, uint y, uint z,
  uint width, uint height, uint depth,
  void* data
);