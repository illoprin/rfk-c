#include "window.h"
#include "input.h"
#include "log.h"

GLFWwindow* handle;
ivec2 wnd_Size;
WindowFnResize uResizeCallback;
bool rawInputSupport = false;
bool mouseGrabbed = false;
bool grabbedBeforeFocusLoss = false;

void sizeCallback(GLFWwindow* _, int width, int height) {
  *wnd_Size = width;
  *(wnd_Size + 1) = height;

  if (uResizeCallback) {
    uResizeCallback(width, height);
  }

  printf("window resized %d %d\n", wnd_Size[0], wnd_Size[1]);
};

void focusCallback(GLFWwindow* _, int focused) {
  if (!focused && mouseGrabbed) {
    Wnd_ToggleMouseGrab();
    grabbedBeforeFocusLoss = true;
  }
  else if (focused && grabbedBeforeFocusLoss) {
    Wnd_ToggleMouseGrab();
    grabbedBeforeFocusLoss = false;
  }
};

void Wnd_Init(int width, int height, const char* title) {
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
  glfwSetFramebufferSizeCallback(handle, sizeCallback);
  glfwSetWindowFocusCallback(handle, focusCallback);
  _inputInit();
}

void Wnd_Center() {
  const GLFWvidmode* m = glfwGetVideoMode(glfwGetPrimaryMonitor());
  ivec2 pos = {
    m->width / 2 - wnd_Size[0] / 2,
    m->height / 2 - wnd_Size[1] / 2,
  };
  glfwSetWindowPos(handle, pos[0], pos[1]);
}

void Wnd_SetResizeCallback(WindowFnResize c) {
  uResizeCallback = c;
}

void Wnd_ToggleMouseGrab() {
  mouseGrabbed = !mouseGrabbed;
  if (mouseGrabbed) {
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (rawInputSupport)
      glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
  else {
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (rawInputSupport)
      glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
}

bool Wnd_Grabbed() {
  return mouseGrabbed;
}

void Wnd_Update() {
  _inputUpdate();
  glfwPollEvents();
}

void Wnd_SwapBuffers() {
  glfwSwapBuffers(handle);
}

void Wnd_Destroy() {
  LogInfo("window destroyed");
  glfwDestroyWindow(handle);
  glfwTerminate();
}

// -- Getters

int* Wnd_GetSize() {
  return wnd_Size;
}

GLFWwindow* Wnd_GetHandle() {
  return handle;
}

bool Wnd_ShouldClose() {
  return glfwWindowShouldClose(handle);
}

void Wnd_Close() {
  glfwSetWindowShouldClose(handle, true);
}
