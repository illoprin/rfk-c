
#include <kernel/gfx/gfx.h>
#include "imgui.h"

/// @brief init system
/// @param window - our glfw window
void ui_init(GLFWwindow* window);

/// @brief call it before drawing imgui
void ui_begin_frame();

/// @brief allow or deny input
void ui_set_input(bool enabled);

/// @brief render
void ui_end_frame();

/// @brief draw initial ui
void ui_draw_demo();

/// @brief destroy context
void ui_destroy();