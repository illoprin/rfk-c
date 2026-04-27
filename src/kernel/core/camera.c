#include "camera.h"
#include <math.h>

void Cam_UpdateVectors(struct Camera* cam) {
  vec3 front;
  front[0] = cosf(glm_rad(cam->Rotation[0])) * cosf(glm_rad(cam->Rotation[0]));
  front[1] = sinf(glm_rad(cam->Rotation[0]));
  front[2] = sinf(glm_rad(cam->Rotation[1])) * cosf(glm_rad(cam->Rotation[0]));
  glm_vec3_copy(front, cam->Front);
  glm_vec3_normalize(cam->Front);

  vec3 worldUp = { 0.0f, 1.0f, 0.0f };
  glm_vec3_cross(cam->Front, worldUp, cam->Right);
  glm_vec3_normalize(cam->Right);

  glm_vec3_cross(cam->Right, cam->Front, cam->Up);
  glm_vec3_normalize(cam->Up);

  vec3 center;
  glm_vec3_add(cam->Position, cam->Front, center);
  glm_lookat(cam->Position, center, cam->Up, cam->view);
}

float* Cam_GetView(struct Camera* cam) {
  return (float*)cam->view;
}

float* Cam_GetProjection(struct Camera* cam, ivec2 screen) {
  float aspect = (float)screen[0] / (float)screen[1];

  glm_perspective(glm_rad(cam->Fov), aspect, CAM_NEAR, CAM_FAR, cam->proj);

  return (float*)cam->proj;
}