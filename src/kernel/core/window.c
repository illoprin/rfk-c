#include "window.h"
#include "input.h"
#include <rfklib/log.h>

static GLFWwindow* handle;
static ivec2 wnd_Size;
static WindowFnResize uResizeCallback;
static bool rawInputSupport = false;
static bool mouseGrabbed = false;
static bool grabbedBeforeFocusLoss = false;

static void size_callback(GLFWwindow* _, int width, int height) {
  *wnd_Size = width;
  *(wnd_Size + 1) = height;

  if (uResizeCallback) {
    uResizeCallback(width, height);
  }

  printf("window resized %d %d\n", wnd_Size[0], wnd_Size[1]);
};

static void focus_callback(GLFWwindow* _, int focused) {
  if (!focused && mouseGrabbed) {
    wnd_toggle_grab();
    grabbedBeforeFocusLoss = true;
  } else if (focused && grabbedBeforeFocusLoss) {
    wnd_toggle_grab();
    grabbedBeforeFocusLoss = false;
  }
};

void wnd_init(int width, int height, const char* title) {
  // init glfw
  RFK_ASSERT(glfwInit(), "failed to init glfw");

  // set window hints
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // create window and setup context
  wnd_Size[0] = width;
  wnd_Size[1] = height;
  handle = glfwCreateWindow(wnd_Size[0], wnd_Size[1], "game", NULL, NULL);
  RFK_ASSERT(handle != NULL, "failed to init window");
  glfwMakeContextCurrent(handle);

  // check raw input support
  if (glfwRawMouseMotionSupported()) {
    rawInputSupport = true;
  }

  // setup callbacks
  glfwSetFramebufferSizeCallback(handle, size_callback);
  glfwSetWindowFocusCallback(handle, focus_callback);
  hid_init();
}

void wnd_center() {
  const GLFWvidmode* m = glfwGetVideoMode(glfwGetPrimaryMonitor());
  ivec2 pos = {
    m->width / 2 - wnd_Size[0] / 2,
    m->height / 2 - wnd_Size[1] / 2,
  };
  glfwSetWindowPos(handle, pos[0], pos[1]);
}

void wnd_set_resize_callback(WindowFnResize c) {
  uResizeCallback = c;
}

void wnd_toggle_grab() {
  mouseGrabbed = !mouseGrabbed;
  if (mouseGrabbed) {
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (rawInputSupport)
      glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  } else {
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (rawInputSupport)
      glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
}

bool wnd_is_grabbed() {
  return mouseGrabbed;
}

void wnd_update() {
  hid_poll();
  glfwPollEvents();
}

void wnd_swap_buffers() {
  glfwSwapBuffers(handle);
}

void wnd_destroy() {
  LogInfo("window destroyed");
  glfwDestroyWindow(handle);
  glfwTerminate();
}

// -- Getters

int* wnd_get_size() {
  return wnd_Size;
}

GLFWwindow* wnd_get_handle() {
  return handle;
}

bool wnd_should_close() {
  return glfwWindowShouldClose(handle);
}

void wnd_close() {
  glfwSetWindowShouldClose(handle, true);
}
