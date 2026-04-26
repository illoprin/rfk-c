#include "render_device.h"

uint _currentBinding = 0;

bool vaoValidate(struct rhi_VAO t) {

  if (!rhi_VAO_IsValid(t)) {
    return false;
  }

  if (_currentBinding == t.ID) {
    return true;
  }

  _currentBinding = t.ID;
  glBindVertexArray(_currentBinding);
  return true;
}

void rhi_RenderDevice_InvalidateBindings() {
  _currentBinding = 0;
  glBindVertexArray(0);
}

void rhi_RenderDevice_Draw(struct rhi_VAO t, uint count) {
  if (!vaoValidate(t)) {
    return;
  }
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
}