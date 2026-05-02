#pragma once

#include "util.h"
#include "texture.h"
#include <rfklib/vector.h>

typedef struct {
  GLuint ID;
  uint width, height;

  Vector colorTextures;    // array of struct rhi_Texture
  rhi_Texture depthTexture;
  bool hasDepth;
} rhi_Fbo;

/// @brief creates framebuffer object
/// @param fbo handle
/// @param width initial width
/// @param height initial height
void rhi_fbo_init(rhi_Fbo* fbo, uint width, uint height);

/// @brief adds color attachment texture to fbo
/// @param fbo handle
/// @param format internal format (e.g. GL_RGBA8)
/// @param filter sampler filtering
void rhi_fbo_add_color(rhi_Fbo* fbo, rhi_TextureFormat format, rhi_TextureFilter filter);

/// @brief adds depth attachment texture to fbo
/// @param fbo handle
/// @param format internal depth format (e.g. GL_DEPTH_COMPONENT24)
void rhi_fbo_add_depth(rhi_Fbo* fbo, rhi_TextureFormat format);

/// @brief sets which color attachments to use for drawing
/// @param fbo handle
/// @param attachments array of attachment indices (0, 1, 2...)
/// @param count number of attachments in array
void rhi_fbo_set_draw_bufs(rhi_Fbo* fbo, uint* attachments, int count);

/// @brief resizes all attachments
void rhi_fbo_resize(rhi_Fbo* fbo, uint width, uint height);

/// @brief blits content from this fbo to another fbo by handle
/// @param src source fbo
/// @param dstId destination raw opengl handle (can be 0 for screen)
/// @param mask bitwise mask (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
void rhi_fbo_blit(rhi_Fbo* src, GLuint dstId, GLbitfield mask);

/// @brief deletes fbo and all attached textures
void rhi_fbo_invalidate(rhi_Fbo* fbo);
