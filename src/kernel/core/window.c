#include "window.h"
#include "input.h"
#include "log.h"

GLFWwindow* handle;
ivec2 wnd_Size;
WindowFnResize uResizeCallback;

void sizeCallback(GLFWwindow* _w, int width, int height) {
  *wnd_Size = width;
  *(wnd_Size + 1) = height;

  if (uResizeCallback) {
    uResizeCallback(width, height);
  }

  printf("window resized %d %d\n", wnd_Size[0], wnd_Size[1]);
};

void Wnd_Init(int width, int height, const char* title) {
  // init glfw
  if (!glfwInit()) {
    LogErr("failed to init glfw");
    RFK_ASSERT(false);
  }

  // set window hints
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // create window and setup context
  wnd_Size[0] = width;
  wnd_Size[1] = height;
  handle = glfwCreateWindow(800, 600, "game", NULL, NULL);
  if (!handle) {
    LogErr("failed to init window");
    RFK_ASSERT(false);
  }
  glfwMakeContextCurrent(handle);

  // init renderer
  if (!gladLoadGL()) {
    LogErr("failed to init context");
    RFK_ASSERT(false);
  }

  // setup callbacks
  glfwSetFramebufferSizeCallback(handle, sizeCallback);
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
