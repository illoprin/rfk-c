#pragma once

#include <kernel/core/window.h>

typedef void (*AppFnVoid)();
typedef void (*AppFnExit)(void** give);
typedef void (*AppFnEnter)(void* receive);
typedef void (*AppFnUpdate)(float deltaTime);

struct StateVTable {
  AppFnVoid Init;
  AppFnUpdate Update;
  AppFnVoid DrawUI;
  AppFnVoid Render;
  AppFnVoid Destroy;
  AppFnExit OnExit;
  AppFnEnter OnEnter;
  WindowFnResize OnResize;
  void* Data;
};

bool hasStateCollision(struct StateVTable a, struct StateVTable b);
void App_Create();
void App_Run();
void App_SetState(struct StateVTable);