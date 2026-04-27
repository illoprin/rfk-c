#include "game.h"

#include <kernel/core/platform.h>
#include <kernel/core/log.h>
#include <kernel/game/defaults/ui.h>

static struct StateVTable currentState = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL
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
  Wnd_Init(800, 600, "game");
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
    Wnd_Update();
    if (currentState.Update) currentState.Update(0.f);
    UI_NewFrame();
    UI_DrawDemoWindow();
    if (currentState.Render) currentState.Render();
    UI_EndFrame();
    Wnd_SwapBuffers();
  }
  Game_destroy();
}

void onResize(int width, int height) {
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void Game_SetState(struct StateVTable nextState) {

  // check collision
  if (hasStateCollision(currentState, nextState)) {
    LogErr("state collision not allowed!");
    RFK_ASSERT(false)
  }

  // invalidate current state
  void* prevStateReturn = NULL;
  if (currentState.OnExit) currentState.OnExit(&prevStateReturn);

  // set next state
  currentState = nextState;

  // call state on_enter func
  if (currentState.OnEnter) currentState.OnEnter(prevStateReturn);
}
