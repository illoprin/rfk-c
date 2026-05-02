#include "app.h"

#include "ui/iui_loader.h"
#include "ui/iui_layout.h"
#include "initial_assets.h"
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
  wnd_init(1600, 850, "game");

  // init glad
  rhi_device_init();

  // init initial assets
  ia_init();

  // init imgui
  ui_init(wnd_get_handle());

  // configure window
  wnd_center();
  wnd_set_resize_callback(onResize);

  LogInfo("game created");
}

static void app_update() {
  if (hid_key_pressed(GLFW_KEY_F3)) {
    iui_switch_stats_mode();
  }
}

static void app_draw_ui() {
  iui_draw_stats_overlay();
}

void app_destroy() {
  LogInfo("game exit");
  if (currentState.Destroy) currentState.Destroy();
  ia_destroy();
  ui_destroy();
  wnd_destroy();
}

void app_run() {
  while (!wnd_should_close()) {
    // update
    wnd_update();
    app_update();
    if (currentState.Update) currentState.Update(prof_dt());

    // draw
    ui_begin_frame();
    if (currentState.DrawUI) currentState.DrawUI();
    app_draw_ui();

    // render
    rhi_device_begin_frame();
    if (currentState.Render) currentState.Render();
    ui_render();
    wnd_swap_buffers();

    // update monitor
    prof_update();
  }
  app_destroy();
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