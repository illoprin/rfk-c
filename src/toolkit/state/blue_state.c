#include "blue_state.h"

#include <framework/framework.h>
#include <kernel/gfx/gfx.h>
#include <toolkit/state/pink_state.h>

Camera cam = {
  .Position = {0, 0,     3.f},
  .Rotation = {0, -90.f, 0  },
  .Fov      = 90.f
};

bool sprint = false;
#define PLAYER_SPEED      6.3  // unit per second
#define PLAYER_SPRINT_MUL 1.75
#define PLAYER_SENS       0.07

Mesh        handgunMesh;
Model       handgunModel;
rhi_Texture handgunTexture;
rhi_Program geomProg;

bool initialized = false;

void player_update_movement(float dt) {
  // sprint
  if (hid_key_pressed(GLFW_KEY_LEFT_CONTROL)) { sprint = !sprint; }
  if (hid_key_released(GLFW_KEY_LEFT_CONTROL)) { sprint = !sprint; }

  vec3 dir  = {0.f};
  vec3 move = {0.f};

  float s  = PLAYER_SPEED * dt;
  s       *= sprint ? PLAYER_SPRINT_MUL : 1.0;

  // move
  if (hid_key_down(GLFW_KEY_W)) { glm_vec3_add(dir, cam.Front, dir); }
  if (hid_key_down(GLFW_KEY_S)) { glm_vec3_sub(dir, cam.Front, dir); }

  // strafe
  if (hid_key_down(GLFW_KEY_D)) { glm_vec3_add(dir, cam.Right, dir); }
  if (hid_key_down(GLFW_KEY_A)) { glm_vec3_sub(dir, cam.Right, dir); }

  // up
  if (hid_key_down(GLFW_KEY_SPACE)) {
    glm_vec3_add(dir, (vec3){0.f, 1.f, 0.f}, dir);
  }
  if (hid_key_down(GLFW_KEY_LEFT_SHIFT)) {
    glm_vec3_add(dir, (vec3){0.f, -1.f, 0.f}, dir);
  }

  // apply to camera
  glm_vec3_scale(dir, s, move);
  glm_vec3_add(cam.Position, move, cam.Position);
}

void player_update_look() {
  double dx, dy;
  hid_cursor_delta(&dx, &dy);
  cam.Rotation[0] =
    glm_clamp(cam.Rotation[0] - dy * PLAYER_SENS, -89.0, 89.0);
  cam.Rotation[1] += dx * PLAYER_SENS;
}

void bs_draw_ui() {
  if (!initialized) { return; }

  ImGuiWindowFlags wflags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
    | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

  int*   size   = wnd_get_size();
  ImVec2 wpos   = {UI_PADDING, size[1] - UI_PADDING};
  ImVec2 wpivot = {0.f, 1.f};

  igSetNextWindowPos(wpos, ImGuiCond_Always, wpivot);

  igBegin("##BlueState", NULL, wflags);

  igText("Camera");

  igDragFloat3(
    "Position",
    cam.Position,
    0.1f,
    0,
    0,
    "%.2f",
    ImGuiDragDropFlags_None
  );
  igDragFloat3(
    "Rotation",
    cam.Rotation,
    0.1f,
    0,
    0,
    "%.2f",
    ImGuiDragDropFlags_None
  );

  igEnd();
}

void bs_update(float dt) {
  ImGuiIO* io = igGetIO();

  if (hid_key_pressed(GLFW_KEY_ESCAPE)) { wnd_close(); }
  if (hid_key_pressed(GLFW_KEY_R)) { app_set_state(ps_get_vtable()); }
  if (hid_key_pressed(GLFW_KEY_G)) {
    wnd_toggle_grab();
    ui_set_input(!wnd_is_grabbed());
  }

  if (!initialized) { return; }

  bool can_update_controller =
    wnd_is_grabbed()
    || (hid_mouse_btn_down(RFK_LMB) && !io->WantCaptureMouse);

  if (can_update_controller) {
    player_update_movement(dt);
    player_update_look();
  }
  cam_update(&cam, wnd_get_size());
}

void bs_render() {
  rhi_device_clear(
    (vec4){0.31, 0.68, 0.9, 1.0},
    RHI_COLOR_BIT | RHI_DEPTH_BIT
  );

  if (!initialized) return;

  rhi_device_use_program(geomProg);
  rhi_prog_uniform_mat4(geomProg, "u_projection", cam.Proj[0]);
  rhi_prog_uniform_mat4(geomProg, "u_view", cam.View[0]);
  rhi_device_bind_tex(handgunTexture, 0);
  rhi_prog_uniform_1i(geomProg, "u_diffuse", 0);
  rhi_device_draw(handgunMesh.VAO, handgunMesh.indexNum);

  ia_render_inf_grid(&cam, .1f, 1.f, 10.f);
}

void bs_init() {
  fputc('\n', stdout);

  RFK_ASSERT(
    rhi_prog_quick_load(
      &geomProg,
      FLS_SHADER_PATH("g_basic.vert"),
      FLS_SHADER_PATH("g_basic.frag")
    ),
    "failed to load state program"
  );

  mdl_init_from_obj(&handgunModel, FLS_MODEL_PATH("handgun.obj"));
  mesh_init_from_model(&handgunMesh, &handgunModel);

  Image2D img;
  RFK_ASSERT(
    !img_from_file(&img, FLS_TEXTURE_PATH("handgun.png")),
    "failed read handgun texture"
  );

  rhi_tex_init(&handgunTexture, RHI_TEX_2D);
  rhi_TextureConfig conf = {
    .MagFilter = RHI_TEX_FILTER_NEAREST,
    .MinFilter = RHI_TEX_FILTER_LINEAR_MIPMAP_LINEAR,
    .Fmt       = RHI_TEX_FORMAT_RGBA8,
    .SrcType   = RHI_UNSIGNED_BYTE,
    .Wrap      = RHI_TEX_WRAP_CLAMP_TO_EDGE,
    .Width     = img.Width,
    .Height    = img.Height,
  };
  rhi_tex2d_alloc(&handgunTexture, conf, img.Pix);
  rhi_tex_gen_mips(handgunTexture);

  img_destroy(img);

  cam_update(&cam, wnd_get_size());

  initialized = true;
}

void bs_destroy() {
  // release resources
  fputc('\n', stdout);
  mesh_invalidate(&handgunMesh);
  mdl_free(handgunModel);
  rhi_tex_invalidate(&handgunTexture);
  rhi_prog_invalidate(&geomProg);
}

void bs_on_exit(void** _) {
  printf("state change\n");
}

void bs_on_enter(void*) {
  if (!initialized) bs_init();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
}

void bs_on_resize(int w, int h) {
  glViewport(0, 0, w, h);
}

StateVTable bs_get_vtable() {
  return (StateVTable){
    .Update         = bs_update,
    .RenderGeometry = bs_render,
    .DrawUI         = bs_draw_ui,
    .OnEnter        = bs_on_enter,
    .OnExit         = bs_on_exit,
    .OnResize       = bs_on_resize,
    .Destroy        = bs_destroy,
    .Init           = bs_init,
  };
}