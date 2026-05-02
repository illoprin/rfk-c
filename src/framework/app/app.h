#pragma once

#include <kernel/core/window.h>

typedef void (*AppFnVoid)();
typedef void (*AppFnExit)(void** give);
typedef void (*AppFnEnter)(void* receive);
typedef void (*AppFnUpdate)(float deltaTime);

typedef struct {
  AppFnVoid Init;
  AppFnUpdate Update;
  AppFnVoid DrawUI;
  AppFnVoid Render;
  AppFnVoid Destroy;
  AppFnExit OnExit;
  AppFnEnter OnEnter;
  WindowFnResize OnResize;
  void* Data;
} StateVTable;

bool app_has_state_collision(StateVTable a, StateVTable b);
void app_create();
void app_run();
void app_set_state(StateVTable);