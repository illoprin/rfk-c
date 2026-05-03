#pragma once

#include "texture.h"
#include "util.h"
#include <rfklib/vector.h>

typedef struct {
  GLuint ID;
  uint   width, height;

  Vector      colorTextures;  // array of struct rhi_Texture
  rhi_Texture depthTexture;
  bool        hasDepth;
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
void rhi_fbo_add_color(
  rhi_Fbo* fbo, rhi_TextureFormat format, rhi_TextureFilter filter
);

/// @brief adds depth attachment texture to fbo
/// @param fbo handle
/// @param format internal depth format (e.g. GL_DEPTH_COMPONENT24)
void rhi_fbo_add_depth(rhi_Fbo* fbo, rhi_TextureFormat format);

/// @brief sets which color attachments to use for drawing
/// @param fbo handle
/// @param attachments array of attachment indices (0, 1, 2...)
/// @param count number of attachments in array
void rhi_fbo_set_draw_bufs(
  rhi_Fbo* fbo, uint* attachments, int count
);

/// @brief validate framebuffer
/// @param fbo handle
/// @return 0 - if completed; 1 - if not complete
bool rhi_fbo_check(rhi_Fbo*);

/// @brief resizes all attachments
void rhi_fbo_resize(rhi_Fbo* fbo, uint width, uint height);

/// @brief deletes fbo and all attached textures
void rhi_fbo_invalidate(rhi_Fbo* fbo);
