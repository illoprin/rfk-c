#pragma once

#include <kernel/gfx/gfx.h>
#include "util.h"

/// @brief init system
/// @param window - our glfw window
void UI_Init(GLFWwindow* window);

/// @brief call it before drawing imgui
void UI_NewFrame();

/// @brief render
void UI_EndFrame();

/// @brief draw initial ui
void UI_DrawDemoWindow();

/// @brief destroy context
void UI_Destroy();