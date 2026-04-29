#pragma once

#include <kernel/core/types.h>
#include <kernel/core/log.h>
#include <glad/glad.h>

// --- Buffer roles (usage semantics) ---
typedef enum : uint {
  RHI_BUF_VERTEX = GL_ARRAY_BUFFER,
  RHI_BUF_INDEX = GL_ELEMENT_ARRAY_BUFFER,
  RHI_BUF_UNIFORM = GL_UNIFORM_BUFFER,
} rhi_BufferType;

// --- Update model ---
typedef enum : uint {
  RHI_USAGE_STATIC = GL_STATIC_DRAW, // GPU read only
  RHI_USAGE_DYNAMIC = GL_DYNAMIC_DRAW, // CPU -> GPU (SubData)
  RHI_USAGE_STREAM = GL_STREAM_DRAW
} rhi_BufferUsage;

// --- Format ---
typedef enum : uint {
  RHI_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  RHI_UNSIGNED_INT = GL_UNSIGNED_INT,
  RHI_INTEGER = GL_INT,
  RHI_FLOAT = GL_FLOAT,
} rhi_DataType;

// --- Shader types ---
typedef enum : uint {
  RHI_SHADER_VERTEX = GL_VERTEX_SHADER,
  RHI_SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
  RHI_SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
} rhi_ShaderType;

// --- Texture Types ---
typedef enum : uint {
  RHI_TEX_2D = GL_TEXTURE_2D,
  RHI_TEX_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  RHI_TEX_CUBE_MAP = GL_TEXTURE_CUBE_MAP
} rhi_TextureType;

// --- Texture filters ---
typedef enum : uint {
  RHI_TEX_FILTER_LINEAR = GL_LINEAR,
  RHI_TEX_FILTER_NEAREST = GL_NEAREST,
  RHI_TEX_FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  RHI_TEX_FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  RHI_TEX_FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  RHI_TEX_FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST
} rhi_TextureFilter;

// --- Texture wrapping ---
typedef enum : uint {
  RHI_TEX_WRAP_REPEAT = GL_REPEAT,
  RHI_TEX_WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  RHI_TEX_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  RHI_TEX_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
} rhi_TextureWrap;

// --- Texture store formats ---
typedef enum : uint {
  // float 4 comp
  RHI_TEX_FORMAT_RGBA16F = GL_RGBA16F,
  RHI_TEX_FORMAT_RGBA32F = GL_RGBA32F,

  // float 3 comp
  RHI_TEX_FORMAT_RGB16F = GL_RGB16F,
  RHI_TEX_FORMAT_RGB32F = GL_RGB32F,

  // float 1 comp
  RHI_TEX_FORMAT_R16F = GL_R16F,
  RHI_TEX_FORMAT_R32F = GL_R32F,

  // byte 4 comp
  RHI_TEX_FORMAT_RGBA8 = GL_RGBA8,

  // byte 3 comp
  RHI_TEX_FORMAT_RGB8 = GL_RGB8,

  // byte 1 comp
  RHI_TEX_FORMAT_R8 = GL_R8,

  // short
  RHI_TEX_FORMAT_DEPTH16 = GL_DEPTH_COMPONENT16,

  // unsigned int
  RHI_TEX_FORMAT_DEPTH24 = GL_DEPTH_COMPONENT24,
  RHI_TEX_FORMAT_DEPTH32 = GL_DEPTH_COMPONENT32,

  // float
  RHI_TEX_FORMAT_DEPTH32F = GL_DEPTH_COMPONENT32F,

  // unsigned_24_8
  RHI_TEX_FORMAT_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
} rhi_TextureFormat;

uint rhi_Util_GetPixelFormat(rhi_TextureFormat fmt);
uint rhi_Util_GetDataType(rhi_TextureFormat fmt);
bool rhi_Util_IsDepthFormat(rhi_TextureFormat fmt);