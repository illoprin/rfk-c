#include "app.h"

#include "ui/initial_ui.h"
#include <kernel/core/core.h>

static StateVTable currentState = { 0 };

void onResize(int width, int height);

bool app_has_state_collision(
  StateVTable a,
  StateVTable b
) {
  return (a.OnExit == b.OnExit && a.OnExit != NULL)
    || (a.OnEnter == b.OnEnter && a.OnEnter != NULL);
}

void app_create() {
  // init platform
  Plt_Init();

  // hello string
  printf(HELLO_STR);

  // create window
  wnd_init(1500, 650, "game");

  // init glad
  rhi_device_init();

  // init imgui
  ui_init(wnd_get_handle());

  // configure window
  wnd_center();
  wnd_set_resize_callback(onResize);

  LogInfo("game created");
}

void App_destroy() {
  LogInfo("game exit");
  if (currentState.Destroy) currentState.Destroy();
  ui_destroy();
  wnd_destroy();
}

void app_run() {
  while (!wnd_should_close()) {
    // update
    wnd_update();
    if (currentState.Update) currentState.Update(prof_dt());

    // draw
    ui_begin_frame();
    ui_draw_demo();
    if (currentState.DrawUI) currentState.DrawUI();

    // render
    rhi_device_begin_frame();
    if (currentState.Render) currentState.Render();
    ui_end_frame();
    wnd_swap_buffers();

    // update monitor
    prof_update();
  }
  App_destroy();
}

void onResize(int width, int height) {
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void app_set_state(StateVTable nextState) {

  // check collision
  RFK_ASSERT(!app_has_state_collision(currentState, nextState), "state collision not allowed");

  // invalidate current state
  void* prevStateReturn = NULL;
  if (currentState.OnExit) currentState.OnExit(&prevStateReturn);

  // set next state
  currentState = nextState;

  // call state on_enter func
  if (currentState.OnEnter) currentState.OnEnter(prevStateReturn);
}