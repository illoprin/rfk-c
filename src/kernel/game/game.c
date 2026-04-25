#include "game.h"

#include <kernel/core/platform.h>

static struct StateVTable currentState = {
  NULL, NULL, NULL, NULL, NULL
};

void onResize(int width, int height);

void Game_Create() {
  printf("🩷 - rfk - with love\n");
  Plt_Init();
  Wnd_Init(800, 600, "game");
  Wnd_Center();
  Wnd_SetResizeCallback(onResize);
}

void Game_Run() {
  while (!Wnd_ShouldClose()) {
    Wnd_Update();
    if (currentState.Update) currentState.Update(0.f);
    if (currentState.Render) currentState.Render();
    Wnd_SwapBuffers();
  }
  Wnd_Destroy();
}

void onResize(int width, int height) {
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void Game_SetState(struct StateVTable nextState) {
  // invalidate current state
  void* prevStateReturn = NULL;
  if (currentState.OnExit) currentState.OnExit(&prevStateReturn);

  // set next state
  currentState = nextState;

  // call state on_enter func
  if (currentState.OnEnter) currentState.OnEnter(prevStateReturn);
}
