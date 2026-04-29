#include "pink_state.h"

#include <toolkit/state/blue_state.h>

void ps_update(float _) {
  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(BlueState_GetVTable());
  }

  if (IsKeyPressed(GLFW_KEY_S)) {
    fputs("it is menu\n", stdout);
  }
}

void ps_render() {
  glClearColor(0.91, 0.49, 0.969, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

struct StateVTable PinkState_GetVTable() {
  return (struct StateVTable) {
    .Update = ps_update,
      .Render = ps_render,
  };
}