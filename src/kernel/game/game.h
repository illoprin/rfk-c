#pragma once

#include <kernel/core/window.h>

typedef void (*GameFnVoid)();
typedef void (*GameFnExit)(void** give);
typedef void (*GameFnEnter)(void* receive);
typedef void (*GameFnUpdate)(float deltaTime);

struct StateVTable {
  GameFnVoid Render;
  GameFnExit OnExit;
  GameFnEnter OnEnter;
  GameFnUpdate Update;
  WindowFnResize OnResize;
  void* Data;
};

void Game_Create();
void Game_Run();
void Game_SetState(struct StateVTable);