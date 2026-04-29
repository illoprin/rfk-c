#include "framebuffer.h"

void rhi_Fbo_Init(struct rhi_Fbo* fbo, uint width, uint height) {
  if (fbo == NULL) return;
  *fbo = (struct rhi_Fbo){ 0 };

  fbo->width = width;
  fbo->height = height;
  Vec_Init(&fbo->colorTextures, struct rhi_Texture);

  glGenFramebuffers(1, &fbo->ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);

  LogInfo("fbo [ID = %d] initialized", fbo->ID);
}

void rhi_Fbo_AddColorAttachment(struct rhi_Fbo* fbo, rhi_TextureFormat format, rhi_TextureFilter filtering) {
  if (fbo == NULL || fbo->ID == 0) return;

  struct rhi_Texture tex;
  rhi_Tex_Init(&tex, RHI_TEX_2D);

  struct rhi_TextureConfig conf = {
      .Format = format,
      .SourceType = rhi_Util_GetDataType(format),
      .FilterMin = filtering,
      .FilterMag = filtering,
      .Wrap = GL_CLAMP_TO_EDGE,
      .Width = fbo->width,
      .Height = fbo->height
  };
  rhi_Tex2D_Allocate(&tex, conf, NULL);

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

void rhi_Fbo_AddDepthAttachment(struct rhi_Fbo* fbo, rhi_TextureFormat format) {
  if (fbo == NULL || fbo->ID == 0) return;

  if (!rhi_Util_IsDepthFormat(format)
    && format != RHI_TEX_FORMAT_DEPTH24_STENCIL8) return;

  rhi_Tex_Init(&fbo->depthTexture, RHI_TEX_2D);

  struct rhi_TextureConfig conf = {
      .Format = format,
      .SourceType = rhi_Util_GetDataType(format),
      .FilterMin = RHI_TEX_FILTER_NEAREST,
      .FilterMag = RHI_TEX_FILTER_NEAREST,
      .Wrap = RHI_TEX_WRAP_CLAMP_TO_EDGE,
      .Width = fbo->width,
      .Height = fbo->height
  };

  rhi_Tex2D_Allocate(&fbo->depthTexture, conf, NULL);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->depthTexture.ID, 0);

  fbo->hasDepth = true;
}

void rhi_Fbo_SetDrawBuffers(struct rhi_Fbo* fbo, uint* attachments, int count) {
  if (fbo == NULL || fbo->ID == 0) return;

  GLenum* enums = malloc(sizeof(GLenum) * count);
  for (int i = 0; i < count; i++) {
    enums[i] = GL_COLOR_ATTACHMENT0 + attachments[i];
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glDrawBuffers(count, enums);

  free(enums);
}

void rhi_Fbo_BindForDrawing(struct rhi_Fbo* fbo) {
  if (fbo == NULL || fbo->ID == 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  };
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
  glViewport(0, 0, fbo->width, fbo->height);
}

void rhi_Fbo_Resize(struct rhi_Fbo* fbo, uint width, uint height) {
  if (fbo == NULL || fbo->ID == 0) return;
  fbo->width = width;
  fbo->height = height;

  // resize color attachments
  for (size_t i = 0; i < fbo->colorTextures.Len; i++) {
    struct rhi_Texture* tex = Vec_AtRaw(&fbo->colorTextures, i);
    rhi_Tex2D_Resize(tex, width, height);
  }

  // resize depth
  if (fbo->hasDepth) {
    rhi_Tex2D_Resize(&fbo->depthTexture, width, height);
  }
}

void rhi_Fbo_Blit(struct rhi_Fbo* src, GLuint dstId, GLbitfield mask) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, src->ID);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstId);

  glBlitFramebuffer(
    0, 0, src->width, src->height,
    0, 0, src->width, src->height, // assuming 1:1 scale
    mask,
    GL_NEAREST
  );
}

void rhi_Fbo_Invalidate(struct rhi_Fbo* fbo) {
  if (fbo == NULL || fbo->ID == 0) return;

  // invalidate color textures
  for (size_t i = 0; i < fbo->colorTextures.Len; i++) {
    struct rhi_Texture* tex = Vec_AtRaw(&fbo->colorTextures, i);
    rhi_Tex_Invalidate(tex);
  }
  Vec_Destroy(&fbo->colorTextures);

  // invalidate depth
  if (fbo->hasDepth) {
    rhi_Tex_Invalidate(&fbo->depthTexture);
  }

  glDeleteFramebuffers(1, &fbo->ID);
  LogInfo("fbo [ID = %d] deleted", fbo->ID);
  *fbo = (struct rhi_Fbo){ 0 };
}