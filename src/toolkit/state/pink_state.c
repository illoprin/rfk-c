#include "pink_state.h"

#include <toolkit/state/blue_state.h>

void ps_update(float _) {
  if (hid_key_pressed(GLFW_KEY_ESCAPE)) wnd_close();
  if (hid_key_pressed(GLFW_KEY_R)) app_set_state(bs_get_vtable());
  if (hid_key_pressed(GLFW_KEY_S)) fputs("it is menu\n", stdout);
}

void ps_render() {
  rhi_device_clear(
    (vec4){0.91, 0.49, 0.969, 1.0},
    RHI_COLOR_BIT | RHI_DEPTH_BIT
  );
}

StateVTable ps_get_vtable() {
  return (StateVTable){
    .Update = ps_update,
    .Render = ps_render,
  };
}