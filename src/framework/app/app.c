#include "app.h"

#include "ui/initial_ui.h"
#include <kernel/core/core.h>

static struct StateVTable currentState = { 0 };

void onResize(int width, int height);

bool hasStateCollision(
  struct StateVTable a,
  struct StateVTable b
) {
  return (a.OnExit == b.OnExit && a.OnExit != NULL)
    || (a.OnEnter == b.OnEnter && a.OnEnter != NULL);
}

void App_Create() {
  // init platform
  Plt_Init();

  // hello string
  printf(HELLO_STR);

  // create window
  Wnd_Init(1500, 650, "game");

  // init glad
  rhi_Device_Init();

  // init imgui
  UI_Init(Wnd_GetHandle());

  // configure window
  Wnd_Center();
  Wnd_SetResizeCallback(onResize);

  LogInfo("game created");
}

void App_destroy() {
  LogInfo("game exit");
  if (currentState.Destroy) currentState.Destroy();
  UI_Destroy();
  Wnd_Destroy();
}

void App_Run() {
  while (!Wnd_ShouldClose()) {
    // update
    Wnd_Update();
    if (currentState.Update) currentState.Update(GetDeltaTime());

    // draw
    UI_NewFrame();
    UI_DrawDemoWindow();
    if (currentState.DrawUI) currentState.DrawUI();

    // render
    rhi_Device_NewFrame();
    if (currentState.Render) currentState.Render();
    UI_EndFrame();
    Wnd_SwapBuffers();

    // update monitor
    Mon_Update();
  }
  App_destroy();
}

void onResize(int width, int height) {
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void App_SetState(struct StateVTable nextState) {

  // check collision
  RFK_ASSERT(!hasStateCollision(currentState, nextState), "state collision not allowed");

  // invalidate current state
  void* prevStateReturn = NULL;
  if (currentState.OnExit) currentState.OnExit(&prevStateReturn);

  // set next state
  currentState = nextState;

  // call state on_enter func
  if (currentState.OnEnter) currentState.OnEnter(prevStateReturn);
}