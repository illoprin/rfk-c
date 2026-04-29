#include "kernel/kernel.h"
#include <kernel/game/defaults/util.h>

// WARN shit code
void goImguiAssertHandler(const char* expr, const char* file, int line) {
  printf("ImGui Assert: %s at %s:%d\n", expr, file, line);
}

void stateA_update(float);
void stateA_render();
void stateA_drawUi();
void stateA_onExit(void**);
void stateA_destroy();
void stateB_onEnter(void*);
void stateB_update(float);
void stateB_render();
void stateA_onEnter(void*);

struct Mesh handgunMesh;
struct Model handgunModel;
struct rhi_Texture handgunTexture;
struct rhi_Program geomProg;

struct Camera cam;
bool sprint = false;
#define PLAYER_SPEED 6.3 // unit per second
#define PLAYER_SPRINT_MUL 1.75
#define PLAYER_SENS 0.07

struct StateVTable stateA = {
  .Update = stateA_update,
  .Render = stateA_render,
  .Destroy = stateA_destroy,
  .OnExit = stateA_onExit,
  .OnEnter = stateA_onEnter,
  .DrawUI = stateA_drawUi,
};

struct StateVTable stateB = {
  .Update = stateB_update,
  .Render = stateB_render,
  .OnEnter = stateB_onEnter,
};

void playerMovement() {
  // sprint
  if (IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) sprint = !sprint;
  if (IsKeyReleased(GLFW_KEY_LEFT_CONTROL)) sprint = !sprint;

  vec3 dir = { 0.f };
  vec3 move = { 0.f };

  float s = PLAYER_SPEED * GetDeltaTime();
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

void stateA_update(float _) {

  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }
  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateB);
  }
  if (IsKeyPressed(GLFW_KEY_G)) {
    Wnd_ToggleMouseGrab();
  }

  if (Wnd_Grabbed()) {
    playerMovement();
    playerLookAround();
  }
  Cam_Update(&cam, Wnd_GetSize());
}

void stateA_drawUi() {
  igBegin("Camera", NULL, 0);

  igDragFloat3("Position", cam.Position, 0, 0, 0, "%.2f", ImGuiDragDropFlags_None);
  igDragFloat3("Rotation", cam.Rotation, 0, 0, 0, "%.2f", ImGuiDragDropFlags_None);

  igEnd();
}

void stateA_render() {
  int* screen = Wnd_GetSize();
  glViewport(0, 0, screen[0], screen[1]);
  glClearColor(0.486, 0.627, 0.922, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rhi_RenderDevice_UseProgram(geomProg);
  rhi_Prog_SetMat4f(geomProg, "u_projection", cam.Proj[0]);
  rhi_Prog_SetMat4f(geomProg, "u_view", cam.View[0]);
  rhi_Tex_BindToUnit(handgunTexture, 0);
  // rhi_Prog_SetInt(geomProg, "u_diffuse", 0);
  rhi_RenderDevice_Draw(handgunMesh.VAO, handgunMesh.indexNum);
}

struct prevStateGive {
  float x;
  int val;
} _give;

void stateA_onEnter(void*) {
  if (!Prog_QuickLoad(
    &geomProg,
    FLS_SHADER_PATH("g_basic.vert"),
    FLS_SHADER_PATH("g_basic.frag")
  )) exit(EXIT_FAILURE);
  Mdl_InitFromObj(&handgunModel, FLS_MODEL_PATH("handgun.obj"));
  Mesh_SetupFromModel(&handgunMesh, &handgunModel);

  struct Image2D img = Fls_ReadImage(FLS_TEXTURE_PATH("handgun.png"));

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

  Img_Free(img);

  cam = (struct Camera){ 0 };
  cam.Position[2] = 3.f;
  cam.Rotation[1] = -90.f;
  cam.Fov = 80.f;
  Cam_Update(&cam, Wnd_GetSize());
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void stateA_onExit(void** g) {
  stateA_destroy();

  _give.val = 10;
  _give.x = 0.234f;
  *g = &_give;
  printf("state exit\n");
}

void stateA_destroy() {
  // release resources
  fputc('\n', stdout);
  Mesh_Invalidate(&handgunMesh);
  Mdl_Free(handgunModel);
  rhi_Tex_Invalidate(&handgunTexture);
  rhi_Prog_Invalidate(&geomProg);
}

void stateB_update(float) {
  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateA);
  }

  if (IsKeyPressed(GLFW_KEY_S)) {
    fputs("it is menu\n", stdout);
  }
}

void stateB_render() {
  glClearColor(0.91, 0.49, 0.969, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void stateB_onEnter(void* r) {
  if (r == NULL) return;
  struct prevStateGive receive = *((struct prevStateGive*)r);
  printf("new state enter\n");
  printf("prev state values\n{ x: %.2f, val: %d }\n",
    receive.x, receive.val);
}

int main() {
  Game_Create();
  Game_SetState(stateA);
  Game_Run();
  return EXIT_SUCCESS;
}