#pragma once

#include <kernel/core/types.h>

#define SMOOTHED_DELTA_TIME_ALPHA 0.1
#define FPS_UPDATE_PERIOD .5 // freq fps updating (seconds)

struct MonitorStats {
  double Time;
  float DeltaTime;
  float SmoothedDeltaTime;
  uint FPS;
  uint LastDrawCalls;
  uint LastVertices;
  uint LastTriangles;
};

void Mon_Update();
float GetDeltaTime();
float GetSmoothedDeltaTime();
struct MonitorStats Mon_GetStats();