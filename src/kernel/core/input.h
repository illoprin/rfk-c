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

// human interface device

// init (input manager)
void inp_init();

// update statement
void inp_poll();

void inp_cursor_delta(double* x, double* y);
void inp_cursor_pos(double* x, double* y);

bool inp_mouse_btn_down(int btn);
bool inp_mouse_btn_pressed(int btn);
bool inp_mouse_btn_released(int btn);
void inp_mouse_scroll(double* x, double* y);

bool inp_key_down(int key);
bool inp_key_released(int key);
bool inp_key_pressed(int key);
