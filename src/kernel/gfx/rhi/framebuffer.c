#include "framebuffer.h"

#include <stdlib.h>

void rhi_fbo_init(rhi_Fbo* fbo, uint width, uint height) {
  if (fbo == NULL) return;
  *fbo = (rhi_Fbo){ 0 };

  fbo->width = width;
  fbo->height = height;
  Vec_Init(&fbo->colorTextures, rhi_Texture);

  glGenFramebuffers(1, &fbo->ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);

  LogInfo("fbo [ID = %d] initialized", fbo->ID);
}

void rhi_fbo_add_color(rhi_Fbo* fbo, rhi_TextureFormat format, rhi_TextureFilter filtering) {
  if (fbo == NULL || fbo->ID == 0) return;

  rhi_Texture tex;
  rhi_tex_init(&tex, RHI_TEX_2D);

  rhi_TextureConfig conf = {
   .Fmt = format,
   .SrcType = rhi_util_get_data_type(format),
   .MinFilter = filtering,
   .MagFilter = filtering,
   .Wrap = GL_CLAMP_TO_EDGE,
   .Width = fbo->width,
   .Height = fbo->height
  };
  rhi_tex2d_alloc(&tex, conf, NULL);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0 + fbo->colorTextures.Len,
    GL_TEXTURE_2D,
    tex.ID,
    0
  );

  Vec_Push(&fbo->colorTextures, tex);
}

void rhi_fbo_add_depth(rhi_Fbo* fbo, rhi_TextureFormat format) {
  if (fbo == NULL || fbo->ID == 0) return;

  if (!rhi_util_is_depth_fmt(format)
    && format != RHI_TEX_FORMAT_DEPTH24_STENCIL8) return;

  rhi_tex_init(&fbo->depthTexture, RHI_TEX_2D);

  rhi_TextureConfig conf = {
   .Fmt = format,
   .SrcType = rhi_util_get_data_type(format),
   .MinFilter = RHI_TEX_FILTER_NEAREST,
   .MagFilter = RHI_TEX_FILTER_NEAREST,
   .Wrap = RHI_TEX_WRAP_CLAMP_TO_EDGE,
   .Width = fbo->width,
   .Height = fbo->height
  };

  rhi_tex2d_alloc(&fbo->depthTexture, conf, NULL);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->depthTexture.ID, 0);

  fbo->hasDepth = true;
}

void rhi_fbo_set_draw_bufs(rhi_Fbo* fbo, uint* attachments, int count) {
  if (fbo == NULL || fbo->ID == 0) return;

  GLenum* enums = malloc(sizeof(GLenum) * count);
  for (int i = 0; i < count; i++) {
    enums[i] = GL_COLOR_ATTACHMENT0 + attachments[i];
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glDrawBuffers(count, enums);

  free(enums);
}

void rhi_fbo_bind_for_drawing(rhi_Fbo* fbo) {
  if (fbo == NULL || fbo->ID == 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  };
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glViewport(0, 0, fbo->width, fbo->height);
}

void rhi_fbo_resize(rhi_Fbo* fbo, uint width, uint height) {
  if (fbo == NULL || fbo->ID == 0) return;
  fbo->width = width;
  fbo->height = height;

  // resize color attachments
  for (size_t i = 0; i < fbo->colorTextures.Len; i++) {
    rhi_Texture* tex = Vec_GetRaw(&fbo->colorTextures, i);
    rhi_tex2d_resize(tex, width, height);
  }

  // resize depth
  if (fbo->hasDepth) {
    rhi_tex2d_resize(&fbo->depthTexture, width, height);
  }
}

void rhi_fbo_blit(rhi_Fbo* src, GLuint dstId, GLbitfield mask) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, src->ID);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstId);

  glBlitFramebuffer(
    0, 0, src->width, src->height,
    0, 0, src->width, src->height, // assuming 1:1 scale
    mask,
    GL_NEAREST
  );
}

void rhi_fbo_invalidate(rhi_Fbo* fbo) {
  if (fbo == NULL || fbo->ID == 0) return;

  // invalidate color textures
  for (size_t i = 0; i < fbo->colorTextures.Len; i++) {
    rhi_Texture* tex = Vec_GetRaw(&fbo->colorTextures, i);
    rhi_tex_invalidate(tex);
  }
  Vec_Destroy(&fbo->colorTextures);

  // invalidate depth
  if (fbo->hasDepth) {
    rhi_tex_invalidate(&fbo->depthTexture);
  }

  glDeleteFramebuffers(1, &fbo->ID);
  LogInfo("fbo [ID = %d] deleted", fbo->ID);
  *fbo = (rhi_Fbo){ 0 };
}