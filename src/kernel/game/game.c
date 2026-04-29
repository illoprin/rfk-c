#include "game.h"

#include <kernel/game/defaults/ui.h>
#include <kernel/core/core.h>

static struct StateVTable currentState = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

void onResize(int width, int height);

bool hasStateCollision(
  struct StateVTable a,
  struct StateVTable b
) {
  return (a.OnExit == b.OnExit && a.OnExit != NULL)
    || (a.OnEnter == b.OnEnter && a.OnEnter != NULL);
}

void Game_Create() {
  printf("🩷 - rfk - with love\n");
  Plt_Init();
  Wnd_Init(1500, 650, "game");
  glBindVertexArray(0);
  UI_Init(Wnd_GetHandle());
  Wnd_Center();
  Wnd_SetResizeCallback(onResize);

  LogInfo("game created");
}

void Game_destroy() {
  LogInfo("game exit");
  if (currentState.Destroy) currentState.Destroy();
  UI_Destroy();
  Wnd_Destroy();
}

void Game_Run() {
  while (!Wnd_ShouldClose()) {
    // update
    Wnd_Update();
    if (currentState.Update) currentState.Update(GetDeltaTime());

    // draw
    UI_NewFrame();
    UI_DrawDemoWindow();
    if (currentState.DrawUI) currentState.DrawUI();

    // render
    rhi_RenderDevice_NewFrame();
    if (currentState.Render) currentState.Render();
    UI_EndFrame();
    Wnd_SwapBuffers();

    // update monitor
    Mon_Update();
  }
  Game_destroy();
}

void onResize(int width, int height) {
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void Game_SetState(struct StateVTable nextState) {

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
