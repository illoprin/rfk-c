#include "blue_state.h"

#include <kernel/core/core.h>
#include <toolkit/state/pink_state.h>
#include <kernel/gfx/gfx.h>
#include <framework/framework.h>

struct Camera cam = {
  .Position = { 0, 0, 3.f },
  .Rotation = { 0, -90.f, 0 },
  .Fov = 90.f
};
bool sprint = false;
#define PLAYER_SPEED 6.3 // unit per second
#define PLAYER_SPRINT_MUL 1.75
#define PLAYER_SENS 0.07

struct Mesh handgunMesh;
struct Model handgunModel;
struct rhi_Texture handgunTexture;
struct rhi_Program geomProg;

bool initialized = false;

void playerMovement(float dt) {
  // sprint
  if (IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) sprint = !sprint;
  if (IsKeyReleased(GLFW_KEY_LEFT_CONTROL)) sprint = !sprint;

  vec3 dir = { 0.f };
  vec3 move = { 0.f };

  float s = PLAYER_SPEED * dt;
  s *= sprint ? PLAYER_SPRINT_MUL : 1.0;

  // move
  if (IsKeyDown(GLFW_KEY_W)) {
    glm_vec3_add(dir, cam.Front, dir);
  }
  if (IsKeyDown(GLFW_KEY_S)) {
    glm_vec3_sub(dir, cam.Front, dir);
  }

  // strafe
  if (IsKeyDown(GLFW_KEY_D)) {
    glm_vec3_add(dir, cam.Right, dir);
  }
  if (IsKeyDown(GLFW_KEY_A)) {
    glm_vec3_sub(dir, cam.Right, dir);
  }

  // up
  if (IsKeyDown(GLFW_KEY_SPACE)) {
    glm_vec3_add(dir, (vec3) { 0.f, 1.f, 0.f }, dir);
  }
  if (IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
    glm_vec3_add(dir, (vec3) { 0.f, -1.f, 0.f }, dir);
  }

  // apply to camera
  glm_vec3_scale(dir, s, move);
  glm_vec3_add(cam.Position, move, cam.Position);
}

void playerLookAround() {
  double dx, dy;
  MouseGetDelta(&dx, &dy);
  cam.Rotation[0] = glm_clamp(cam.Rotation[0] - dy * PLAYER_SENS, -89.0, 89.0);
  cam.Rotation[1] += dx * PLAYER_SENS;
}

void bs_drawUI() {
  if (!initialized) return;

  igBegin("Camera", NULL, 0);

  igDragFloat3("Position", cam.Position, 0, 0, 0, "%.2f", ImGuiDragDropFlags_None);
  igDragFloat3("Rotation", cam.Rotation, 0, 0, 0, "%.2f", ImGuiDragDropFlags_None);

  igEnd();
}

void bs_update(float deltaTime) {
  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }
  if (IsKeyPressed(GLFW_KEY_R)) {
    App_SetState(PinkState_GetVTable());
  }
  if (IsKeyPressed(GLFW_KEY_G)) {
    Wnd_ToggleMouseGrab();
  }

  if (!initialized) return;

  ImGuiIO* io = igGetIO();


  if (Wnd_Grabbed()) {
    playerMovement(deltaTime);
    playerLookAround();
    igSetNextFrameWantCaptureKeyboard(false);
    igSetNextFrameWantCaptureMouse(false);
  }
  Cam_Update(&cam, Wnd_GetSize());
}

void bs_render() {
  int* screen = Wnd_GetSize();
  glViewport(0, 0, screen[0], screen[1]);
  glClearColor(0.31, 0.68, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!initialized) return;

  rhi_Device_UseProgram(geomProg);
  rhi_Prog_SetMat4f(geomProg, "u_projection", cam.Proj[0]);
  rhi_Prog_SetMat4f(geomProg, "u_view", cam.View[0]);
  rhi_Tex_BindToUnit(handgunTexture, 0);
  rhi_Prog_SetInt(geomProg, "u_diffuse", 0);
  rhi_Device_Draw(handgunMesh.VAO, handgunMesh.indexNum);
}

void bs_init() {
  fputc('\n', stdout);
  if (!Prog_QuickLoad(
    &geomProg,
    FLS_SHADER_PATH("g_basic.vert"),
    FLS_SHADER_PATH("g_basic.frag")
  )) exit(EXIT_FAILURE);
  Mdl_InitFromObj(&handgunModel, FLS_MODEL_PATH("handgun.obj"));
  Mesh_SetupFromModel(&handgunMesh, &handgunModel);

  struct Image2D img;
  RFK_ASSERT(!Img_FromFile(&img, FLS_TEXTURE_PATH("handgun.png")), "failed read handgun texture");

  rhi_Tex_Init(&handgunTexture, RHI_TEX_2D);
  struct rhi_TextureConfig conf = {
    .FilterMag = RHI_TEX_FILTER_NEAREST,
    .FilterMin = RHI_TEX_FILTER_LINEAR_MIPMAP_LINEAR,
    .Format = RHI_TEX_FORMAT_RGBA8,
    .SourceType = RHI_UNSIGNED_BYTE,
    .Wrap = RHI_TEX_WRAP_CLAMP_TO_EDGE,
    .Width = img.Width,
    .Height = img.Height,
  };
  rhi_Tex2D_Allocate(&handgunTexture, conf, img.Pix);
  rhi_Tex_GenMipmaps(handgunTexture);

  Img_Destroy(img);

  Cam_Update(&cam, Wnd_GetSize());

  initialized = true;
}

void bs_destroy() {
  // release resources
  fputc('\n', stdout);
  Mesh_Invalidate(&handgunMesh);
  Mdl_Free(handgunModel);
  rhi_Tex_Invalidate(&handgunTexture);
  rhi_Prog_Invalidate(&geomProg);
};

void bs_onExit(void** _) {
  printf("state change\n");
}

void bs_onEnter(void*) {
  if (!initialized) {
    bs_init();
  }

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
}

struct StateVTable BlueState_GetVTable() {
  return (struct StateVTable) {
    .Update = bs_update,
      .Render = bs_render,
      .DrawUI = bs_drawUI,
      .OnEnter = bs_onEnter,
      .OnExit = bs_onExit,
      .Destroy = bs_destroy,
      .Init = bs_init,
  };
}