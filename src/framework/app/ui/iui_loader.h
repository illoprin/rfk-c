
#include "imgui.h"
#include <kernel/gfx/gfx.h>

// Initial UI (iui) module

/// @brief init system
/// @param window - our glfw window
void iui_init(GLFWwindow* window);

/// @brief call it before drawing imgui
void ui_begin_frame();

/// @brief allow or deny input
void ui_set_input(bool enabled);

/// @brief render
void ui_render();

/// @brief destroy context
void ui_destroy();