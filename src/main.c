#include "util.h"

#include "window.h"
#include "input.h"
#include "log.h"
#include "platform.h"

#define VECTOR_H_IMPLEMENTATION
#include <vector/vector.h>

void update() {
  Wnd_Update();

  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyDown(GLFW_KEY_W)) {
    fputs("forward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_S)) {
    fputs("backward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_A)) {
    fputs("strafe left\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_D)) {
    fputs("strafe right\n", stdout);
  }
  if (IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint start\n", stdout);
  }
  if (IsKeyReleased(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint end\n", stdout);
  }
  if (IsMouseButtonPressed(RFK_LMB)) {
    fputs("selection start \n", stdout);
  }
  if (IsMouseButtonReleased(RFK_LMB)) {
    fputs("selection end\n", stdout);
  }
  if (IsMouseButtonDown(RFK_RMB)) {
    double dx, dy, x, y;
    MouseGetDelta(&dx, &dy);
    MouseGetPos(&x, &y);
    printf("mouse x:%.2f y:%.2f dx:%.2f dy:%.2f\n", x, y, dx, dy);
  }
}

void render() {
  glClearColor(0.486, 0.627, 0.922, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO render code here ...
  Wnd_SwapBuffers();
}


int main() {

  Plt_Init();

  Wnd_Init(800, 600, "game");
  Wnd_Center();


  while (!Wnd_ShouldClose()) {
    update();
    render();
  }

  Wnd_Destroy();


  return EXIT_SUCCESS;
}