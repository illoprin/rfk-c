#include "kernel/kernel.h"

void update(float);
void render();
void onExit(void**);
void onEnter(void*);
void updateNext(float);
void renderNext();

struct StateVTable stateA = {
  .Update = update,
  .Render = render,
  .OnExit = onExit,
};

struct StateVTable stateB = {
  .Update = updateNext,
  .Render = renderNext,
  .OnEnter = onEnter,
};

void update(float _) {

  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateB);
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

void render() {
  glClearColor(0.486, 0.627, 0.922, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO render code here ...
}

struct prevStateGive {
  float x;
  int val;
} _give;

void onExit(void** g) {
  _give.val = 10;
  _give.x = 0.234f;
  *g = &_give;
  printf("state exit\n");
}

void updateNext(float) {
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

void renderNext() {
  glClearColor(0.91, 0.49, 0.969, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void onEnter(void* r) {
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