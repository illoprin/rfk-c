#pragma once

#include <kernel/core/types.h>

#define SMOOTHED_DELTA_TIME_ALPHA 0.1
#define FPS_UPDATE_PERIOD .5 // freq fps updating (seconds)

struct MonitorStats {
  double Time;
  float DeltaTime;
  float SmoothedDeltaTime;
  int FPS;
  int LastDrawCalls;
  int LastVertices;
  int LastTriangles;
};

void Mon_Update();
float GetDeltaTime();
float GetSmoothedDeltaTime();
struct MonitorStats Mon_GetStats();