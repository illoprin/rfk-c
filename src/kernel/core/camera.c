#include "camera.h"
#include <math.h>

void cam_update(Camera* cam, ivec2 screen) {
  // compute direction vectors
  float pitch = glm_rad(cam->Rotation[0]),
        yaw   = glm_rad(cam->Rotation[1]),
        roll  = glm_rad(cam->Rotation[2]);
  vec3 front;
  front[0] = cosf(pitch) * cosf(yaw);
  front[1] = sinf(pitch);
  front[2] = sinf(yaw) * cosf(pitch);
  glm_vec3_copy(front, cam->Front);
  glm_vec3_normalize(cam->Front);

  vec3 worldUp = {0.0f, 1.0f, 0.0f};
  glm_vec3_cross(cam->Front, worldUp, cam->Right);
  glm_vec3_normalize(cam->Right);

  glm_vec3_cross(cam->Right, cam->Front, cam->Up);
  glm_vec3_normalize(cam->Up);

  // compute roll influence
  if (roll != 0) {
    // rotate vectors by roll
    float rcos = cosf(roll);
    float rsin = sinf(roll);

    vec3 a, b, c;

    // right = normalize(right * rcos + cross(front, right) * rsin)
    glm_vec3_scale(cam->Right, rcos, a);
    glm_vec3_cross(cam->Front, cam->Right, b);
    glm_vec3_scale(b, rsin, c);
    glm_vec3_add(a, c, cam->Right);
    glm_vec3_normalize(cam->Right);

    // up = normalize(cross(right, front))
    glm_vec3_cross(cam->Right, cam->Front, cam->Up);
    glm_vec3_normalize(cam->Up);
  }

  // compute matrices
  vec3 center;
  glm_vec3_add(cam->Position, cam->Front, center);
  glm_lookat(cam->Position, center, cam->Up, cam->View);

  float aspect = (float)screen[0] / (float)screen[1];
  glm_perspective(
    glm_rad(cam->Fov),
    aspect,
    CAM_NEAR,
    CAM_FAR,
    cam->Proj
  );
}
