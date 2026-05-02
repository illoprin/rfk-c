#pragma once

#include <cglm/cglm.h> 

#define CAM_NEAR (.01f)
#define CAM_FAR (100.f)
#define CAM_FOV (90.f)

#define DIR_UP (vec3){0.f, 1.f, 0.f}

typedef struct {
  // transforms
  vec3 Position;
  vec3 Rotation;

  // directions
  vec3 Front;
  vec3 Right;
  vec3 Up;

  // matrices
  float Fov;
  mat4 Proj;
  mat4 View;
} Camera;

void cam_update(Camera*, ivec2 screen);