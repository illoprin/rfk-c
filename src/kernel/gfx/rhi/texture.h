#pragma once

#include <glad/glad.h>
#include <kernel/core/types.h>
#include "buffer.h"

typedef enum {
  RHI_TEX_FILTER_LINEAR = GL_LINEAR,
  RHI_TEX_FILTER_NEAREST = GL_NEAREST,
  RHI_TEX_FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  RHI_TEX_FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  RHI_TEX_FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  RHI_TEX_FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST
} rhi_TextureFilter;

typedef enum {
  RHI_TEX_WRAP_REPEAT = GL_REPEAT,
  RHI_TEX_WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  RHI_TEX_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  RHI_TEX_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
} rhi_TextureWrap;

typedef enum {
  RHI_TEX_FORMAT_RGBA8 = GL_RGBA8,
  RHI_TEX_FORMAT_RGBA16F = GL_RGBA16F,
  RHI_TEX_FORMAT_RGBA32F = GL_RGBA32F,
  RHI_TEX_FORMAT_RGB8 = GL_RGB8,
  RHI_TEX_FORMAT_RGB16F = GL_RGB16F,
  RHI_TEX_FORMAT_RGB32F = GL_RGB32F,
  RHI_TEX_FORMAT_R8 = GL_R8,
  RHI_TEX_FORMAT_R16F = GL_R16F,
  RHI_TEX_FORMAT_R32F = GL_R32F,
  RHI_TEX_FORMAT_DEPTH16 = GL_DEPTH_COMPONENT16,
  RHI_TEX_FORMAT_DEPTH24 = GL_DEPTH_COMPONENT24,
  RHI_TEX_FORMAT_DEPTH32F = GL_DEPTH_COMPONENT32F,
  RHI_TEX_FORMAT_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
} rhi_TextureFormat;

/**
 * @brief rhi texture types for internal validation
 */
typedef enum {
  RHI_TEXTURE_2D = GL_TEXTURE_2D,
  RHI_TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  RHI_TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP

} rhi_TextureType;

/**
 * @brief container for immutable storage parameters
 */
struct rhi_TextureConfig {
  rhi_TextureFormat Format;
  rhi_TextureFilter FilterMin;
  rhi_TextureFilter FilterMag;
  rhi_TextureWrap Wrap;
};

/**
 * @brief texture handle and type descriptor
 */
struct rhi_Texture {
  uint32_t handle;
  rhi_TextureType type;
};

/// @brief binds texture into sampler unit
/// @param tex target texture object
/// @param unit index of sampler unit
void rhi_Tex_BindToUnit(struct rhi_Texture tex, uint unit);

/**
 * @brief creates an immutable 2d texture storage
 * @param w texture width
 * @param h texture height
 * @param conf storage and sampler configuration
 * @returns initialized rhi_Texture object
 */
struct rhi_Texture rhi_Tex2D_Create(int w, int h, struct rhi_TextureConfig conf);

/**
 * @brief updates a sub-region of a 2d texture
 * @param tex target texture object
 * @param x offset x
 * @param y offset y
 * @param w region width
 * @param h region height
 * @param pixel_format source data format (e.g. GL_RGBA)
 * @param pixel_type source data type (e.g. GL_UNSIGNED_BYTE)
 * @param data pointer to pixel data
 */
void rhi_Tex2D_Update(struct rhi_Texture tex, int x, int y, int w, int h, uint p_fmt, rhi_DataType p_type, const void* data);

/**
 * @brief creates an immutable 2d array texture storage
 * @param w texture width
 * @param h texture height
 * @param layers number of slices in array
 * @param conf storage and sampler configuration
 * @returns initialized rhi_Texture object
 */
struct rhi_Texture rhi_Tex2DArray_Create(int w, int h, int layers, struct rhi_TextureConfig conf);

/**
 * @brief updates a slice of a 2d array texture using 3d dsa call
 * @param tex target texture object
 * @param x offset x
 * @param y offset y
 * @param layer slice index
 * @param w region width
 * @param h region height
 * @param p_fmt source data format
 * @param p_type source data type
 * @param data pointer to pixel data
 */
void rhi_Tex2DArray_Update(struct rhi_Texture tex, int x, int y, int layer, int w, int h, uint p_fmt, rhi_DataType p_type, const void* data);

/**
 * @brief creates an immutable cube map storage
 * @param size width and height of each face
 * @param conf storage and sampler configuration
 * @returns initialized rhi_Texture object
 */
struct rhi_Texture rhi_TexCube_Create(int size, struct rhi_TextureConfig conf);

/**
 * @brief updates cube map faces using glTextureSubImage3D where depth represents faces
 * @param tex target cube map object
 * @param x offset x
 * @param y offset y
 * @param face starting face index (0-5)
 * @param w region width
 * @param h region height
 * @param count number of faces to update
 * @param p_fmt source data format
 * @param p_type source data type
 * @param data pointer to pixel data
 */
void rhi_TexCube_Update(struct rhi_Texture tex, int x, int y, int face, int w, int h, int count, uint p_fmt, rhi_DataType p_type, const void* data);

/**
 * @brief releases gpu texture resource
 */
void rhi_Tex_Destroy(struct rhi_Texture tex);