#pragma once

#include "vao.h"

void rhi_RenderDevice_InvalidateBindings();
void rhi_RenderDevice_Draw(struct rhi_VAO, uint count);