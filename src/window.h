#pragma once

#include <gfx.h>
#include <util.h>
#include <input.h>

typedef void(*FuncWindowResize)(int width, int height);

void Wnd_Init(int width, int height, const char* title);
void Wnd_Center();
void Wnd_SetResizeCallback(FuncWindowResize);
void Wnd_Update();
void Wnd_SwapBuffers();
void Wnd_Close();
void Wnd_Destroy();

GLFWwindow* Wnd_GetHandle();
bool Wnd_ShouldClose();
int* Wnd_GetSize();