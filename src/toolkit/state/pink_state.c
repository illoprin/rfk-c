#include "pink_state.h"

#include <toolkit/state/blue_state.h>

void ps_update(float _) {
  if (inp_key_pressed(GLFW_KEY_ESCAPE)) wnd_close();
  if (inp_key_pressed(GLFW_KEY_R)) app_set_state(bs_get_vtable());
  if (inp_key_pressed(GLFW_KEY_S)) fputs("it is menu\n", stdout);
}

void ps_render() {
  rhi_device_clear(RHI_COLOR_BIT | RHI_DEPTH_BIT);
}

StateVTable ps_get_vtable() {
  return (StateVTable){
    .Update = ps_update,
    .RenderGeometry = ps_render,
  };
}