#pragma once

#include "util.h"
#include "texture.h"
#include <vector/vector.h>

struct rhi_Fbo {
  GLuint ID;
  uint width, height;

  Vector colorTextures;    // array of struct rhi_Texture
  struct rhi_Texture depthTexture;
  bool hasDepth;
};

/// @brief creates framebuffer object
/// @param fbo handle
/// @param width initial width
/// @param height initial height
void rhi_Fbo_Init(struct rhi_Fbo* fbo, uint width, uint height);

/// @brief adds color attachment texture to fbo
/// @param fbo handle
/// @param format internal format (e.g. GL_RGBA8)
/// @param filter sampler filtering
void rhi_Fbo_AddColorAttachment(struct rhi_Fbo* fbo, rhi_TextureFormat format, rhi_TextureFilter filter);

/// @brief adds depth attachment texture to fbo
/// @param fbo handle
/// @param format internal depth format (e.g. GL_DEPTH_COMPONENT24)
void rhi_Fbo_AddDepthAttachment(struct rhi_Fbo* fbo, rhi_TextureFormat format);

/// @brief sets which color attachments to use for drawing
/// @param fbo handle
/// @param attachments array of attachment indices (0, 1, 2...)
/// @param count number of attachments in array
void rhi_Fbo_SetDrawBuffers(struct rhi_Fbo* fbo, uint* attachments, int count);

/// @brief binds fbo and sets viewport
void rhi_Fbo_BindForDrawing(struct rhi_Fbo* fbo);

/// @brief resizes all attachments
void rhi_Fbo_Resize(struct rhi_Fbo* fbo, uint width, uint height);

/// @brief blits content from this fbo to another fbo by handle
/// @param src source fbo
/// @param dstId destination raw opengl handle (can be 0 for screen)
/// @param mask bitwise mask (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
void rhi_Fbo_Blit(struct rhi_Fbo* src, GLuint dstId, GLbitfield mask);

/// @brief deletes fbo and all attached textures
void rhi_Fbo_Invalidate(struct rhi_Fbo* fbo);
