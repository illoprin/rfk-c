#pragma once

#include <kernel/gfx/gfx.h>
#include "input.h"

typedef void(*WindowFnResize)(int width, int height);

void wnd_init(int width, int height, const char* title);
void wnd_center();
void wnd_set_resize_callback(WindowFnResize);
void wnd_update();
void wnd_swap_buffers();
void wnd_close();
void wnd_destroy();
void wnd_toggle_grab();
bool wnd_is_grabbed();

GLFWwindow* wnd_get_handle();
bool wnd_should_close();
int* wnd_get_size();