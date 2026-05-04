#include "app.h"

#include "initial_assets.h"
#include "ui/iui_layout.h"
#include "ui/iui_loader.h"
#include <framework/config.h>
#include <framework/gfx/pipeline/deferred.h>
#include <framework/gfx/pipeline/rendering_pipeline.h>
#include <kernel/core/core.h>

static StateVTable currentState = {0};

static ivec2 screen_size = {0};

void onResize(int width, int height);

bool app_has_state_collision(StateVTable a, StateVTable b) {
  return (a.OnExit == b.OnExit && a.OnExit != NULL)
         || (a.OnEnter == b.OnEnter && a.OnEnter != NULL);
}

void app_create() {
  // init platform
  Plt_Init();

  // hello string
  printf(HELLO_STR);

  // create window
  wnd_init(WINDOW_WIDTH, WINDOW_HEIGHT, "game");
  screen_size[0] = (int)((float)WINDOW_WIDTH * SCREEN_SIZE_RATIO);
  screen_size[1] = (int)((float)WINDOW_HEIGHT * SCREEN_SIZE_RATIO);

  // init glad
  rhi_device_init();

  // init initial assets
  ia_init();

  // init rendering pipeline
  rpl_init();

  // init imgui
  iui_init(wnd_get_handle());

  // configure window
  wnd_center();
  wnd_set_resize_callback(onResize);

  LogInfo("game created");
}

static void app_update() {
  if (hid_key_pressed(GLFW_KEY_F3)) { iuil_switch_stats_mode(); }
}

static void app_draw_ui() {
  iuil_draw_stats_overlay();
}

void app_destroy() {
  LogInfo("game destroy");
  if (currentState.Destroy) currentState.Destroy();
  ia_destroy();
  rpl_destroy();
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
    if (currentState.RenderGeometry) {
      rpl_render(currentState.RenderGeometry);
    }
    ui_render();
    wnd_swap_buffers();

    // update monitor
    prof_update();
  }
  app_destroy();
}

void onResize(int width, int height) {
  screen_size[0] = (float)screen_size[0] * SCREEN_SIZE_RATIO;
  screen_size[1] = (float)screen_size[1] * SCREEN_SIZE_RATIO;

  rpl_resize(width, height);
  if (currentState.OnResize) currentState.OnResize(width, height);
}

void app_set_state(StateVTable nextState) {

  // check collision
  RFK_ASSERT(
    !app_has_state_collision(currentState, nextState),
    "state collision not allowed"
  );

  // invalidate current state
  void* prevStateReturn = NULL;
  if (currentState.OnExit) currentState.OnExit(&prevStateReturn);

  // set next state
  currentState = nextState;

  // call state on_enter func
  if (currentState.OnEnter) currentState.OnEnter(prevStateReturn);
}

int* app_get_screen_size() {
  return screen_size;
}
