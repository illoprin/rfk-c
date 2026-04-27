#include "kernel/kernel.h"

// WARN shit code
void goImguiAssertHandler(const char* expr, const char* file, int line) {
  printf("ImGui Assert: %s at %s:%d\n", expr, file, line);
}

void stateA_update(float);
void stateA_render();
void stateA_onExit(void**);
void stateA_destroy();
void stateB_onEnter(void*);
void stateB_update(float);
void stateB_render();
void stateA_onEnter(void*);

struct Mesh meshQuad;

struct rhi_Program program;

struct StateVTable stateA = {
  .Update = stateA_update,
  .Render = stateA_render,
  .Destroy = stateA_destroy,
  .OnExit = stateA_onExit,
  .OnEnter = stateA_onEnter,
};

struct StateVTable stateB = {
  .Update = stateB_update,
  .Render = stateB_render,
  .OnEnter = stateB_onEnter,
};

void stateA_update(float _) {

  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateB);
  }

  if (IsKeyPressed(GLFW_KEY_G)) {
    Wnd_ToggleMouseGrab();
  }

  if (IsKeyDown(GLFW_KEY_W)) {
    fputs("forward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_S)) {
    fputs("backward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_A)) {
    fputs("strafe left\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_D)) {
    fputs("strafe right\n", stdout);
  }
  if (IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint start\n", stdout);
  }
  if (IsKeyReleased(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint end\n", stdout);
  }
  if (IsMouseButtonPressed(RFK_LMB)) {
    fputs("selection start \n", stdout);
  }
  if (IsMouseButtonReleased(RFK_LMB)) {
    fputs("selection end\n", stdout);
  }
  if (IsMouseButtonDown(RFK_RMB)) {
    double dx, dy, x, y;
    MouseGetDelta(&dx, &dy);
    MouseGetPos(&x, &y);
    printf("mouse x:%.2f y:%.2f dx:%.2f dy:%.2f\n", x, y, dx, dy);
  }
}

void stateA_render() {
  glClearColor(0.486, 0.627, 0.922, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  rhi_Prog_Use(program);
  rhi_RenderDevice_InvalidateBindings();
  rhi_RenderDevice_Draw(meshQuad.VAO, 6);
}

struct prevStateGive {
  float x;
  int val;
} _give;

void stateA_onEnter(void*) {
  Prog_QuickLoad(
    &program,
    FLS_SHADER_PATH("basic.vert"),
    FLS_SHADER_PATH("basic.frag")
  );
  Mesh_SetupBasicQuad(&meshQuad);
}

void stateA_onExit(void** g) {
  stateA_destroy();

  _give.val = 10;
  _give.x = 0.234f;
  *g = &_give;
  printf("state exit\n");
}
void stateA_destroy() {
  // release resources
  Mesh_Invalidate(&meshQuad);
  rhi_Prog_Destroy(&program);
}

void stateB_update(float) {
  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateA);
  }

  if (IsKeyPressed(GLFW_KEY_S)) {
    fputs("it is menu\n", stdout);
  }
}

void stateB_render() {
  glClearColor(0.91, 0.49, 0.969, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void stateB_onEnter(void* r) {
  struct prevStateGive receive = *((struct prevStateGive*)r);
  printf("new state enter\n");
  printf("prev state values\n{ x: %.2f, val: %d }\n",
    receive.x, receive.val);
}

int main() {
  Game_Create();
  Game_SetState(stateA);
  Game_Run();
  return EXIT_SUCCESS;
}