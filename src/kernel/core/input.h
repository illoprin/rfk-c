#pragma once

#include <kernel/gfx/gfx.h>

enum BtnState {
  RFK_NONE = 0,
  RFK_PRESSED,
  RFK_DOWN,
  RFK_RELEASED,
};

enum {
  RFK_LMB = GLFW_MOUSE_BUTTON_1,
  RFK_RMB = GLFW_MOUSE_BUTTON_2,
};

void _inputInit();
void _inputUpdate();

void MouseGetDelta(double* x, double* y);
void MouseGetPos(double* x, double* y);
void MouseGetScroll(double* x, double* y);

bool IsMouseButtonDown(int btn);
bool IsMouseButtonPressed(int btn);
bool IsMouseButtonReleased(int btn);

bool IsKeyDown(int key);
bool IsKeyReleased(int key);
bool IsKeyPressed(int key);
