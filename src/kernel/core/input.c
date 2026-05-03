#include "input.h"
#include "window.h"
#include <string.h>

struct keyboard {
  int keys[GLFW_KEY_LAST];
};

struct mouse {
  int    btns[GLFW_MOUSE_BUTTON_LAST];
  double xscr, yscr;
  double x, y, dx, dy;
};

static struct mouse    mouse;
static struct keyboard keyboard;
static GLFWwindow*     handle;

void keyCallback(
  GLFWwindow* _w, int key, int scancode, int action, int mods
);
void mouseButtonCallback(
  GLFWwindow* _w, int button, int action, int mods
);
void cursorPosCallback(GLFWwindow* _w, double x, double y);
void scrollCallback(GLFWwindow* _w, double x, double y);

void windowSetupCallbacks() {
  glfwSetKeyCallback(handle, keyCallback);
  glfwSetMouseButtonCallback(handle, mouseButtonCallback);
  glfwSetCursorPosCallback(handle, cursorPosCallback);
  glfwSetScrollCallback(handle, scrollCallback);
}

void hid_init() {
  memset(keyboard.keys, RFK_NONE, GLFW_KEY_LAST * sizeof(int));
  memset(mouse.btns, RFK_NONE, GLFW_MOUSE_BUTTON_LAST * sizeof(int));
  handle = wnd_get_handle();
  windowSetupCallbacks();
}

bool hid_key_down(int key) {
  return keyboard.keys[key] == RFK_DOWN
         || keyboard.keys[key] == RFK_PRESSED;
}

bool hid_key_released(int key) {
  return keyboard.keys[key] == RFK_RELEASED;
}

bool hid_key_pressed(int key) {
  return keyboard.keys[key] == RFK_PRESSED;
}

bool hid_mouse_btn_down(int btn) {
  return mouse.btns[btn] == RFK_PRESSED
         || mouse.btns[btn] == RFK_DOWN;
}

bool hid_mouse_btn_pressed(int btn) {
  return mouse.btns[btn] == RFK_PRESSED;
}

bool hid_mouse_btn_released(int btn) {
  return mouse.btns[btn] == RFK_RELEASED;
}

void hid_cursor_delta(double* x, double* y) {
  *x = mouse.dx;
  *y = mouse.dy;
}

void hid_cursor_pos(double* x, double* y) {
  *x = mouse.x;
  *y = mouse.y;
}

void hid_mouse_scroll(double* x, double* y) {
  *x = mouse.xscr;
  *y = mouse.yscr;
}

void hid_poll() {
  // keyboard update
  for (size_t i = 0; i < GLFW_KEY_LAST; i++) {
    if (keyboard.keys[i] == RFK_PRESSED) {
      keyboard.keys[i] = RFK_DOWN;
    } else if (keyboard.keys[i] == RFK_RELEASED) {
      keyboard.keys[i] = RFK_NONE;
    }
  }

  // mouse button update
  for (size_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
    if (mouse.btns[i] == RFK_PRESSED) {
      mouse.btns[i] = RFK_DOWN;
    } else if (mouse.btns[i] == RFK_RELEASED) {
      mouse.btns[i] = RFK_NONE;
    }
  }

  // mouse delta update
  mouse.dx = 0;
  mouse.dy = 0;

  // mouse scroll update
  mouse.xscr = 0;
  mouse.yscr = 0;
}

void keyCallback(
  GLFWwindow* _w, int key, int scancode, int action, int mods
) {
  if (key < 0) { return; }

  if (action == GLFW_PRESS) {
    keyboard.keys[key] = RFK_PRESSED;
  } else if (action == GLFW_RELEASE) {
    keyboard.keys[key] = RFK_RELEASED;
  }
}

void mouseButtonCallback(
  GLFWwindow* _w, int button, int action, int mods
) {
  if (button < 0) { return; }

  if (action == GLFW_PRESS) {
    mouse.btns[button] = RFK_PRESSED;
  } else if (action == GLFW_RELEASE) {
    mouse.btns[button] = RFK_RELEASED;
  }
}

void cursorPosCallback(GLFWwindow* _w, double x, double y) {
  mouse.dx = x - mouse.x;
  mouse.dy = y - mouse.y;
  mouse.x  = x;
  mouse.y  = y;
}

void scrollCallback(GLFWwindow* _w, double x, double y) {
  mouse.xscr = x;
  mouse.yscr = y;
}