#include "kernel/kernel.h"

void stateA_update(float);
void stateA_render();
void stateA_onExit(void**);
void stateA_destroy();
void stateB_onEnter(void*);
void stateB_update(float);
void stateB_render();
void stateA_onEnter(void*);

struct StateVTable stateA = {
  .Update = stateA_update,
  .Render = stateA_render,
  .Destroy = stateA_destroy,
  .OnExit = stateA_onExit,
  .OnEnter = stateA_onEnter,
};

struct StateVTable stateB = {
  .Update = stateB_update,
  .Render = stateB_render,
  .OnEnter = stateB_onEnter,
};


static struct rhi_Buffer bufQuadVertex;
static struct rhi_Buffer bufQuadIndex;
static struct rhi_VAO vaoQuad;

void stateA_update(float _) {

  if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Wnd_Close();
  }

  if (IsKeyPressed(GLFW_KEY_R)) {
    Game_SetState(stateB);
  }

  if (IsKeyDown(GLFW_KEY_W)) {
    fputs("forward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_S)) {
    fputs("backward\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_A)) {
    fputs("strafe left\n", stdout);
  }
  if (IsKeyDown(GLFW_KEY_D)) {
    fputs("strafe right\n", stdout);
  }
  if (IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint start\n", stdout);
  }
  if (IsKeyReleased(GLFW_KEY_LEFT_SHIFT)) {
    fputs("sprint end\n", stdout);
  }
  if (IsMouseButtonPressed(RFK_LMB)) {
    fputs("selection start \n", stdout);
  }
  if (IsMouseButtonReleased(RFK_LMB)) {
    fputs("selection end\n", stdout);
  }
  if (IsMouseButtonDown(RFK_RMB)) {
    double dx, dy, x, y;
    MouseGetDelta(&dx, &dy);
    MouseGetPos(&x, &y);
    printf("mouse x:%.2f y:%.2f dx:%.2f dy:%.2f\n", x, y, dx, dy);
  }
}

void stateA_render() {
  glClearColor(0.486, 0.627, 0.922, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  rhi_RenderDevice_InvalidateBindings();
  rhi_RenderDevice_Draw(vaoQuad, 6);
}

struct prevStateGive {
  float x;
  int val;
} _give;

void stateA_onExit(void** g) {
  stateA_destroy();

  _give.val = 10;
  _give.x = 0.234f;
  *g = &_give;
  printf("state exit\n");
}

void stateA_destroy() {
  // release resources
  rhi_Buffer_Invalidate(&bufQuadIndex);
  rhi_Buffer_Invalidate(&bufQuadVertex);
  rhi_VAO_Destroy(&vaoQuad);
}

void stateA_onEnter(void*) {

  float basicQuadVertices[] = {
    -.5f, -.5f,
    .5f, -.5f,
    .5f, .5f,
    -.5f, .5f,
  };
  uint basicQuadIndices[] = {
    0, 1, 2,
    2, 3, 0,
  };

  // allocate resources
  rhi_Buffer_Create(
    &bufQuadVertex,
    sizeof(basicQuadVertices),
    basicQuadVertices,
    RHI_BUFFER_VERTEX,
    RHI_USAGE_STATIC
  );
  rhi_Buffer_Create(
    &bufQuadIndex,
    sizeof(basicQuadIndices),
    basicQuadIndices,
    RHI_BUFFER_INDEX,
    RHI_USAGE_STATIC
  );

  rhi_VAO_Create(&vaoQuad);
  rhi_VAO_BindVertexBuffer(&vaoQuad, 0, bufQuadVertex.ID, 0, sizeof(float) * 2);
  rhi_VAO_SetAttribute(&vaoQuad,
    (struct rhi_Attribute) {
    .Location = 0,
      .BindingIndex = 0,
      .Components = 2,
      .Type = RHI_ATTR_FLOAT,
      .Normalized = false,
      .Offset = 0,
  });
  rhi_VAO_SetIndexBuffer(&vaoQuad, bufQuadIndex.ID);

  RFK_ASSERT(rhi_VAO_IsValid(vaoQuad));
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