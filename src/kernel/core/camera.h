#pragma once

#include "util.h"

#define CAM_NEAR (.01f)
#define CAM_FAR (100.f)
#define CAM_FOV (90.f)

#define DIR_UP (vec3){0.f, 1.f, 0.f}

struct Camera {
  vec3 Position;
  vec3 Rotation;
  vec3 Front;
  vec3 Right;
  vec3 Up;
  float Fov;
};

float* Cam_GetProjection(struct Camera*, ivec2 screen);
float* Cam_GetView(struct Camera*);
void Cam_UpdateVectors(struct Camera*);